#include <iostream>

#include "netutils.hpp"
namespace pack {

uint64_t pack754(long double f, unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0) return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }

    // get the normalized form of f and track the exponent
    shift = 0;
    while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL<<significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

    // return the final answer
    return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
}

long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) return 0.0;

    // pull the significand
    result = (i&((1LL<<significandbits)-1)); // mask
    result /= (1LL<<significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1)) - 1;
    shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
    while(shift > 0) { result *= 2.0; shift--; }
    while(shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (i>>(bits-1))&1? -1.0: 1.0;

    return result;
}

void packu32(unsigned char *buf, uint32_t i)
{
    *buf++ = i>>24; *buf++ = i>>16;
    *buf++ = i>>8;  *buf++ = i;
}

uint32_t unpacku32(unsigned char *buf)
{
    uint32_t i = ((uint32_t)buf[0]<<24) |
                           ((uint32_t)buf[1]<<16) |
                           ((uint32_t)buf[2]<<8)  |
                           buf[3];
    return i;
}



void encode_pos(unsigned char *buf, float x, float y)
{
    uint32_t x_enc = pack754_32(x);
    uint32_t y_enc = pack754_32(y);

    packu32(buf, x_enc);
    packu32(buf + 4, y_enc);
}


std::pair<float, float> decode_pos(unsigned char *buf)
{
    uint32_t x_int = unpacku32(buf);
    uint32_t y_int = unpacku32(buf + sizeof(uint32_t));

    return std::make_pair(unpack754_32(x_int), unpack754_32(y_int));
}

std::pair<int16_t, int16_t> decode_input(unsigned char *buf)
{
    int16_t x, y;
    x = ((int16_t)buf[0] << 8) | ((int16_t)buf[1]);
    y = ((int16_t)buf[2] << 8) | ((int16_t)buf[3]);
    return std::make_pair(x, y);
}


void encode_input(unsigned char *buf, int16_t x, int16_t y)
{
    uint32_t out = ((uint32_t)x << 16) + (uint32_t)y;
    packu32(buf, out);
}

} // namespace pack

namespace net {
std::pair<std::string, int> get_addr_and_port(SOCKADDR_STORAGE *sock_a)
{
    char *client_ip;
    short port;
    if (sock_a->ss_family == AF_INET) {
        client_ip = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
        SOCKADDR_IN *addr = (SOCKADDR_IN *)sock_a;

        inet_ntop(AF_INET, &(addr->sin_addr), client_ip, INET_ADDRSTRLEN);

        port = ntohs(addr->sin_port);
    } else {
        SOCKADDR_IN6 *addr = (SOCKADDR_IN6 *)sock_a;
        client_ip = (char *)malloc(INET6_ADDRSTRLEN * sizeof(char));

        inet_ntop(AF_INET6, &(addr->sin6_addr), client_ip, INET6_ADDRSTRLEN);
        port = ntohs(addr->sin6_port);
    }
    std::pair<std::string, int> out = std::make_pair(
        std::string(client_ip), port);
    free(client_ip);
    return out;
}

ADDRINFOA *addr_info(const std::string &addr, int port, int sock_type)
{
    int status;
    ADDRINFOA hints;
    ADDRINFOA *out;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(addr.c_str(), std::to_string(port).c_str(),
        &hints, &out)) != 0) {
        throw NetworkException("can't obtain address structure");
    }
    return out;
}

void send_buf(int sock, unsigned char *buf, ssize_t len)
{
    if (send(sock, (char *)buf, len, 0) == -1) {
        throw NetworkException("can't send data");
    }
}

bool recv_buf(int sock, unsigned char *buf, ssize_t len)
{
    ssize_t received = recvtimeout(sock, (char *)buf, len, 0);
    if (received == -2) {
        return false;
    }
    if (received == -1) {
        throw NetworkException("can't receive");
    }
    ssize_t recv_len = received;
    while (received < len) {
        received = recvtimeout(sock, (char *)buf + recv_len, len - recv_len, 1000);
        if (received == -2) {
            return false;
        }
        if (received == -1) {
            throw NetworkException("can't receive");
        }
        recv_len += received;
    }
    return true;
}

int recvtimeout(int s, char *buf, int len, int timeout)
{
    fd_set fds;
    int n;
    struct timeval tv;

    // set up the file descriptor set
    FD_ZERO(&fds);
    FD_SET(s, &fds);

    // set up the struct timeval for the timeout
    tv.tv_sec = 0;
    tv.tv_usec = timeout;

    // wait until timeout or data received
    n = select(s+1, &fds, NULL, NULL, &tv);
    if (n == 0) return -2; // timeout!
    if (n == -1) return -1; // error

    // data must be here, so do a normal recv()
    return recv(s, buf, len, 0);
}
} // namespace net

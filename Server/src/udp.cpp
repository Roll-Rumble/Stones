#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <utility>

#define NUMBYTES 8

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

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

std::pair<int16_t, int16_t> decode_input(unsigned char *buf)
{
    int16_t x, y;
    x = ((int16_t)buf[0] << 8) | ((int16_t)buf[1]);
    y = ((int16_t)buf[2] << 8) | ((int16_t)buf[3]);
    return std::make_pair(x, y);
}

void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

// returns socket file descriptor
int init_udp_conn(const char *addr, int port)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        error("can't create socket");
    }

    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            error("can't set UDP socket to be reusable");
    }

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = (in_port_t)htons(port);
    client_addr.sin_addr.s_addr = inet_addr(addr);

    if (connect(sockfd, (struct sockaddr *) &client_addr,
        sizeof(client_addr)) == -1) {
        error("can't connect");
    }
}


void send_xy(int sockfd, float x, float y)
{
    unsigned char buf[NUMBYTES];
    encode_pos(buf, x, y);

    send(sockfd, buf, NUMBYTES, 0);
}

std::pair<int16_t, int16_t> recv_xy(int sockfd)
{
    unsigned char buf[4];
    recv(sockfd, buf, 4, 0);
    return decode_input(buf);
}


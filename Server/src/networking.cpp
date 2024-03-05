#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <utility>
#include <iostream>

#include "networking.hpp"



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

void error(std::string msg)
{
	std::cerr << msg << ": " << strerror(errno) << std::endl;
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

    return sockfd;
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



std::pair<std::string, short> wait_for_tcp() {
    std::cout << "We're in TCP server..." << std::endl;

    // Select server port
    int server_port = 13000;

    // Create welcome socket
    int welcome_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcome_socket == -1) { // If socket creation fails, print error message and exit
        std::cerr << "Socket creation failed" << std::endl;
        throw "Socket creation failed";
    }

    // Bind server to local host at port server_port (12000)
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address; INADDR_ANY allows to have multiple clients connected
    server_addr.sin_port = htons(server_port); 

    if (bind(welcome_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        error("binding to socket failed");
    }

    listen(welcome_socket, 1);

    // Message to indicate that server is running
    std::cout << "Server running on port " << server_port << std::endl;

    sockaddr_in client_addr; // Client address
    socklen_t client_len = sizeof(client_addr);

    int connection_socket = accept(welcome_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len); // Accept the connection

    if (connection_socket == -1) {
        error("can't accept connection");
    }



    
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	close(connection_socket);
    close(welcome_socket);
    short port = ntohs(client_addr.sin_port);

	std::cout << std::string(client_ip) << std::endl;
	return std::make_pair(std::string(client_ip), port);
}


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

#include "netutils.hpp"
#include "networking.hpp"


// returns socket file descriptor
UDPServ::UDPServ(std::string &addr)
{
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ == -1) {
        throw NetworkException("can't create socket");
    }

    int reuse = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            throw NetworkException("can't set UDP socket to be reusable");
    }

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = (in_port_t)htons(SEND_PORT);
    client_addr.sin_addr.s_addr = inet_addr(addr.c_str());

    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = (in_port_t)htons(LISTEN_PORT);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd_, (struct sockaddr *)&local_addr, sizeof(local_addr)) == -1) {
        throw NetworkException("can't bind socket");
    }

    if (connect(sockfd_, (struct sockaddr *) &client_addr,
        sizeof(client_addr)) == -1) {
        throw NetworkException("can't connect to socket");
    }

}

UDPServ::~UDPServ()
{
    close(sockfd_);
}


void UDPServ::send_xy(float x, float y)
{
    std::cout << "sending " << x << ", " << y << std::endl;
    unsigned char buf[NUM_OUT_BYTES];
    pack::encode_pos(buf, x, y);

    send(sockfd_, buf, NUM_OUT_BYTES, 0);
}

std::pair<int16_t, int16_t> UDPServ::recv_xy()
{
    unsigned char buf[4];
    recv(sockfd_, buf, 4, 0);
    return pack::decode_input(buf);
}



std::pair<std::string, short> wait_for_tcp() {

    // Select server port
    int server_port = 13000;

    // Create welcome socket
    int welcome_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcome_socket == -1) { // If socket creation fails, print error message and exit
        throw NetworkException("Socket creation failed");
    }

    int reuse = 1;
    if (setsockopt(welcome_socket, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            throw NetworkException("can't set UDP socket to be reusable");
    }
    // Bind server to local host at port server_port (12000)
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0"); // Accept connections from any IP address; INADDR_ANY allows to have multiple clients connected
    server_addr.sin_port = htons(server_port); 

    if (bind(welcome_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        throw NetworkException("binding to socket failed");
    }

    if (listen(welcome_socket, 1) == -1) {
        throw NetworkException("can't listen for tcp connection");
    }

    // TODO should be sockaddr_storage because sockaddr_in is only for IPv4
    sockaddr_in client_addr; // Client address
    socklen_t client_len = sizeof(client_addr);

    int connection_socket = accept(welcome_socket, (sockaddr *)(&client_addr), &client_len); // Accept the connection

    if (connection_socket == -1) {
        throw NetworkException("can't accept connection");
    }

    
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	close(connection_socket);
    close(welcome_socket);
    short port = ntohs(client_addr.sin_port);

	std::cout << std::string(client_ip) << std::endl;
	return std::make_pair(std::string(client_ip), port);
}


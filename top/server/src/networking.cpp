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
UDPServ::UDPServ(std::string &addr, int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        throw NetworkException("can't create socket");
    }

    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            throw NetworkException("can't set UDP socket to be reusable");
    }

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = (in_port_t)htons(port);
    client_addr.sin_addr.s_addr = inet_addr(addr.c_str());

    if (connect(sockfd, (struct sockaddr *) &client_addr,
        sizeof(client_addr)) == -1) {
        throw NetworkException("can't connect to socket");
    }
}

UDPServ::~UDPServ()
{
    close(sockfd);
}


void UDPServ::send_xy(float x, float y)
{
    unsigned char buf[NUM_OUT_BYTES];
    pack::encode_pos(buf, x, y);

    send(sockfd, buf, NUM_OUT_BYTES, 0);
}

std::pair<int16_t, int16_t> UDPServ::recv_xy()
{
    unsigned char buf[4];
    recv(sockfd, buf, 4, 0);
    return pack::decode_input(buf);
}



std::pair<std::string, short> wait_for_tcp() {

    // Select server port
    int server_port = 13000;

    // Create welcome socket
    int welcome_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcome_socket == -1) { // If socket creation fails, print error message and exit
        throw "Socket creation failed";
    }

    // Bind server to local host at port server_port (12000)
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address; INADDR_ANY allows to have multiple clients connected
    server_addr.sin_port = htons(server_port); 

    if (bind(welcome_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        throw NetworkException("binding to socket failed");
    }

    listen(welcome_socket, 1);

    sockaddr_in client_addr; // Client address
    socklen_t client_len = sizeof(client_addr);

    int connection_socket = accept(welcome_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len); // Accept the connection

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


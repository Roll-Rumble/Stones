#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <utility>
#include <iostream>

#include "netutils.hpp"
#include "networking.hpp"


TCPServ::TCPServ()
{
    ADDRINFOA *server_addr_info = net::addr_info(
        "0.0.0.0", TCP_LISTEN_PORT, SOCK_STREAM);
    
    sockfd_ = socket(server_addr_info->ai_family,
        server_addr_info->ai_socktype, server_addr_info->ai_protocol);
    if (sockfd_ == -1) {
        throw NetworkException("can't create socket");
    }

    int reuse = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            throw NetworkException("can't set UDP socket to be reusable");
    }


    if (bind(sockfd_, server_addr_info->ai_addr,
        server_addr_info->ai_addrlen) == -1) {
        throw NetworkException("can't bind socket");
    }

    freeaddrinfo(server_addr_info);
}

TCPServ::~TCPServ()
{
    close(sockfd_);
    for (int sock : conn_socks_) {
        close(sock);
    }
}

std::vector<std::string> TCPServ::get_connections(int num_clients)
{
    if (listen(sockfd_, num_clients) == -1) {
        throw NetworkException("can't listen for tcp connection");
    }

    std::vector<std::string> out;

    sockaddr_storage client_addr; // Client address
    socklen_t client_len = sizeof(client_addr);

    for (int i = 0; i < num_clients; i++) {
        conn_socks_.push_back(accept(sockfd_, (sockaddr *)(&client_addr), &client_len));
        if (conn_socks_[conn_socks_.size()-1] == -1) {
            throw NetworkException("can't accept connection");
        }
        out.push_back(net::get_addr_and_port(&client_addr).first);
    }
    
	return out;
}

// returns socket file descriptor
UDPServ::UDPServ(std::string &addr)
{
    ADDRINFOA *client_addr_info = net::addr_info(
        addr.c_str(), SEND_PORT, SOCK_DGRAM);
    
    ADDRINFOA *server_addr_info = net::addr_info(
        "0.0.0.0", LISTEN_PORT, SOCK_DGRAM);
    
    sockfd_ = socket(client_addr_info->ai_family,
        client_addr_info->ai_socktype, client_addr_info->ai_protocol);
    if (sockfd_ == -1) {
        throw NetworkException("can't create socket");
    }

    int reuse = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            throw NetworkException("can't set UDP socket to be reusable");
    }


    if (bind(sockfd_, server_addr_info->ai_addr,
        server_addr_info->ai_addrlen) == -1) {
        throw NetworkException("can't bind socket");
    }

    if (connect(sockfd_, client_addr_info->ai_addr,
        client_addr_info->ai_addrlen) == -1) {
        throw NetworkException("can't connect to socket");
    }
    freeaddrinfo(client_addr_info);
    freeaddrinfo(server_addr_info);
}

UDPServ::~UDPServ()
{
    close(sockfd_);
}


void UDPServ::send_xy(float x, float y)
{
    unsigned char buf[NUM_OUT_BYTES];
    pack::encode_pos(buf, x, y);

    net::send_buf(sockfd_, buf, NUM_OUT_BYTES);
}

std::pair<int16_t, int16_t> UDPServ::recv_xy()
{
    unsigned char buf[4];
    net::recv_buf(sockfd_, buf, 4);
    return pack::decode_input(buf);
}




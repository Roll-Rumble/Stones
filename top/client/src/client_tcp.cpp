#include "client_tcp.hpp"

#include "netutils.hpp"
#include "winsock_start.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

TCPClient::TCPClient() {
    start_WSA();

    try {
        ADDRINFOA *client_addr_info = net::addr_info(
            SERVER_IP, SERVER_TCP_PORT, SOCK_STREAM);
        
        socket_ = socket(client_addr_info->ai_family,
            client_addr_info->ai_socktype, client_addr_info->ai_protocol);
        if (socket_ == SOCKET_ERROR) {
            throw NetworkException("can't create socket");
        }

        int reuse = 1;
        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
            (char *)&reuse, sizeof(int)) == SOCKET_ERROR) {
                throw NetworkException("can't set UDP socket to be reusable");
        }


        if (connect(socket_, client_addr_info->ai_addr,
            client_addr_info->ai_addrlen) == SOCKET_ERROR) {
            throw NetworkException("can't connect socket");
        }

        freeaddrinfo(client_addr_info);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
}

TCPClient::~TCPClient() {
    closesocket(socket_);
    WSACleanup();
}

void TCPClient::send_data(unsigned char buffer[SEND_BUF_SIZE]) {
    try {
        net::send_buf(socket_, buffer, SEND_BUF_SIZE);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
}

void TCPClient::receive(unsigned char buffer[RECEIVE_BUF_SIZE]) {
    try {
        memset(buffer, 0, RECEIVE_BUF_SIZE);
        net::recv_buf(socket_, buffer, RECEIVE_BUF_SIZE);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
}

void TCPClient::send_xy(int16_t x, int16_t y) {
    unsigned char buffer[4];
    pack::encode_input(buffer, x, y);

    try {
        net::send_buf(socket_, buffer, sizeof(buffer));
    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << "\n";
        wchar_t* s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&s, 0, NULL);
        fprintf(stderr, "%S\n", s);
        LocalFree(s);
        WSACleanup();
        throw e;
    }
}

std::pair<float, float> TCPClient::receive_xy(std::pair<float, float> def) {
    unsigned char buffer[8];
    try {
        if (net::recv_buf(socket_, buffer, sizeof(buffer))) {
            return pack::decode_pos(buffer);
        } else {
            return def;
        }

    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << "\n";
        WSACleanup();
        throw e;
    }
}
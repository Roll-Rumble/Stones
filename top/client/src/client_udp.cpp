#include "client_udp.hpp"

#include "netutils.hpp"
#include "winsock_start.hpp"
#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

UDPClient::UDPClient() {
    start_WSA();

    try {
        ADDRINFOA *client_addr_info = net::addr_info(
            "0.0.0.0", CLIENT_UDP_RECV_PORT, SOCK_DGRAM);
        
        ADDRINFOA *server_addr_info = net::addr_info(
            SERVER_IP, SERVER_UDP_PORT, SOCK_DGRAM);
        
        send_socket_ = socket(client_addr_info->ai_family,
            client_addr_info->ai_socktype, client_addr_info->ai_protocol);
        if (send_socket_ == -1) {
            throw NetworkException("can't create socket");
        }

        int reuse = 1;
        if (setsockopt(send_socket_, SOL_SOCKET, SO_REUSEADDR,
            (char *)&reuse, sizeof(int)) == -1) {
                throw NetworkException("can't set UDP socket to be reusable");
        }


        if (bind(send_socket_, client_addr_info->ai_addr,
            client_addr_info->ai_addrlen) == -1) {
            throw NetworkException("can't bind socket");
        }

        if (connect(send_socket_, server_addr_info->ai_addr,
            server_addr_info->ai_addrlen) == -1) {
            throw NetworkException("can't connect to socket");
        }
        freeaddrinfo(client_addr_info);
        freeaddrinfo(server_addr_info);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
    // Send socket init
    send_socket_ = INVALID_SOCKET;
    send_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_socket_ == SOCKET_ERROR) {
        WSACleanup();
        throw NetworkException("Error creating UDP send socket");
    }
}

UDPClient::~UDPClient() {
    closesocket(send_socket_);
    closesocket(recv_socket_);
    WSACleanup();
}

void UDPClient::send_xy(int16_t x, int16_t y) {
    unsigned char buffer[4];
    pack::encode_input(buffer, x, y);

    try {
        net::send_buf(socket_, buffer, sizeof(buffer));
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
}

std::pair<float, float> UDPClient::receive_xy() {
    unsigned char buffer[8];

    try {
        net::recv_buf(socket_, buffer, sizeof(buffer));
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }

    return pack::decode_pos(buffer);
}

// void UDPClient::send(char buffer[UDP_SEND_BUF_SIZE]) {
//     buffer = htons(buffer);

//     int sbyteCount = sendto(client_socket_, &buffer, sizeof(buffer), 0, (SOCKADDR*) &client_addr, sizeof(client_addr));
//     if (sbyteCount == SOCKET_ERROR) {
//         throw NetworkException;
//     }
// }

// void UDPClient::receive(char buffer[UDP_RECEIVE_BUF_SIZE]) {
//     sockaddr_in sender_addr; // Client address
//     socklen_t sender_len = sizeof(sender_addr);
//     int bytes_received;

//     while (true) {
//         // Receive data from the client
//         bytes_received = recv(socket_, buffer, sizeof(buffer) - 1, 0);
//         if (bytes_received <= 0) { // Error receiving data or client closed connection
//             throw NetworkException("error receiving UDP data");
//         }
//         std::string received_data; // Used to hold the received data
//         // Null-terminate the received data
//         buffer[bytes_received] = 0;
//         // Append received data to a string
//         received_data.append(buffer);
//     }
// }

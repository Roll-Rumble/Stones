#include "client_udp.hpp"

#include "netutils.hpp"
#include "winsock_start.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

UDPClient::UDPClient(uint32_t connection_nb) {
    start_WSA();

    std::cout << "Creating UDP port to listen on port " << CLIENT_UDP_RECV_PORT
              << "and send to address " << SERVER_IP << " at port " << SERVER_UDP_PORT_BASE + connection_nb << "\n";

    try {
        ADDRINFOA *client_addr_info = net::addr_info(
            "0.0.0.0", CLIENT_UDP_RECV_PORT, SOCK_DGRAM);

        ADDRINFOA *server_addr_info = net::addr_info(
            SERVER_IP, SERVER_UDP_PORT_BASE + connection_nb, SOCK_DGRAM);

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

        std::cout << "client_addr_info is " << inet_ntoa(((SOCKADDR_IN *)server_addr_info->ai_addr)->sin_addr) << "\n";
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
}

UDPClient::~UDPClient() {
    closesocket(send_socket_);
    WSACleanup();
}

void UDPClient::send_xy(int16_t x, int16_t y) {
    unsigned char buffer[4];
    pack::encode_input(buffer, x, y);

    try {
        net::send_buf(send_socket_, buffer, sizeof(buffer));
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

std::pair<float, float> UDPClient::receive_xy(std::pair<float, float> def) {
    unsigned char buffer[8];
    try {
        // UDP recv has timeout of 1ms
        if (net::recv_buf(send_socket_, buffer, sizeof(buffer), 1000)) {
            std::cout << "Successfully received a UDP packet\n";
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

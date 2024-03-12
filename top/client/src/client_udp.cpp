#include "client_udp.hpp"

#include "netutils.hpp"
#include "winsock_start.hpp"
#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

UDP_Client::UDP_Client() {
    start_WSA();

    // Send socket init
    send_socket_ = INVALID_SOCKET;
    send_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_socket_ == SOCKET_ERROR) {
        WSACleanup();
        throw NetworkException("Error creating UDP send socket");
    }

    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_UDP_PORT); // Ensure the port is correct
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); // Ensure the IP is correct

    SOCKADDR_IN local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(SERVER_UDP_PORT);
    inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr);

    // shoudl be recv socket
    if (bind(send_socket_, (SOCKADDR *)&local_addr, sizeof(local_addr)) == SOCKET_ERROR) {
        std::cerr << "UDP error code: " << WSAGetLastError() << "\n";
        WSACleanup();
        throw NetworkException("error binding UDP socket: error code ");
    }

    if (connect(send_socket_, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        WSACleanup();
        throw NetworkException("can't connect to UDP socket");
    }

    // // Recv socket init
    // recv_socket_ = INVALID_SOCKET;
    // recv_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    // if (recv_socket_ == SOCKET_ERROR) {
    //     WSACleanup();
    //     throw NetworkException("Error creating UDP recv send socket");
    // }
}

UDP_Client::~UDP_Client() {
    closesocket(send_socket_);
    closesocket(recv_socket_);
    WSACleanup();
}

void UDP_Client::send_xy(int16_t x, int16_t y) {
    unsigned char buffer[4];
    pack::encode_input(buffer, x, y);

    int sbyteCount = send(send_socket_, (char *) buffer, sizeof(buffer), 0);
    if (sbyteCount == SOCKET_ERROR) {
        WSACleanup();
        throw NetworkException("error sending UDP data");
    }
}

std::pair<float, float> UDP_Client::receive_xy() {
    sockaddr_in sender_addr; // Client address
    socklen_t sender_len = sizeof(sender_addr);
    int bytes_received;

    unsigned char buffer[8];

    // Receive data from the client
    // should be recv_socket_
    bytes_received = recv(send_socket_, (char *) buffer, sizeof(buffer), 0);
    if (bytes_received == SOCKET_ERROR) { // Error receiving data or client closed connection
        WSACleanup();
        throw NetworkException("error receiving UDP data");
    }

    return pack::decode_pos(buffer);
}

// void UDP_Client::send(char buffer[UDP_SEND_BUF_SIZE]) {
//     buffer = htons(buffer);

//     int sbyteCount = sendto(client_socket_, &buffer, sizeof(buffer), 0, (SOCKADDR*) &client_addr, sizeof(client_addr));
//     if (sbyteCount == SOCKET_ERROR) {
//         throw NetworkException;
//     }
// }

// void UDP_Client::receive(char buffer[UDP_RECEIVE_BUF_SIZE]) {
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

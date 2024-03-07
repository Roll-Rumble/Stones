#include "client_udp.hpp"

#include "netutils.hpp"
#include "winsock_start.hpp"
#include <cstdint>
#include <winsock2.h>
#include <ws2tcpip.h>

UDP_Client::UDP_Client(int port, std::string server_ip) {
    WSAStartup();
    SocketCreation(socket_);

    SOCKADDR_IN client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port); // Ensure the port is correct
    inet_pton(AF_INET, server_ip.c_str(), &client_addr.sin_addr); // Ensure the IP is correct

    if (connect(socket_, (SOCKADDR *)&client_addr, sizeof(client_addr)) == SOCKET_ERROR) {
        throw NetworkException("can't connect to UDP socket");
    }
}

UDP_Client::~UDP_Client() {
    closesocket(socket_);
    WSACleanup();
}

void UDP_Client::send_xy(int16_t x, int16_t y) {
    unsigned char buffer[4];
    pack::encode_input(buffer, x, y);

    int sbyteCount = send(socket_, (char *) buffer, sizeof(buffer), 0);
    if (sbyteCount == SOCKET_ERROR) {
        throw NetworkException("error sending UDP data");
    }
}

std::pair<float, float> UDP_Client::receive_xy() {
    sockaddr_in sender_addr; // Client address
    socklen_t sender_len = sizeof(sender_addr);
    int bytes_received;

    unsigned char buffer[8];

    // Receive data from the client
    bytes_received = recv(socket_, (char *) buffer, sizeof(buffer), 0);
    if (bytes_received == SOCKET_ERROR) { // Error receiving data or client closed connection
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

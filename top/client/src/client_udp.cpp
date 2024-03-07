#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <arpa/inet.h>
#include "winsock_start.hpp"
#include "client_udp.hpp"

UDP_Client::UDP_Client(int port, std::string server_ip) :
                       port_(port), server_ip_(server_ip) {

    client_service_.sin_family = AF_INET;
    client_service_.sin_port = htons(PORT); // Ensure the port is correct
    inet_pton(AF_INET, server_ip_, &client_service_.sin_addr); // Ensure the IP is correct
}

void UDP_Client::~UDP_Client() {
    closesocket(socket_);
    WSACleanup();
}

void UDP_Client::send(char buffer[UDP_SEND_BUF_SIZE]) {
    buffer = htons(buffer);

    int sbyteCount = sendto(client_socket_, &buffer, sizeof(buffer), 0, (SOCKADDR*) &client_service_, sizeof(client_service_));
    if (sbyteCount == SOCKET_ERROR) {
        std::cout << "Server send error: " << WSAGetLastError() << "\n";
    } else {
        std::cout << "Server: sent " << sbyteCount << " bytes\n";
    }
}

void UDP_Client::receive(int port, std::string server_ip, char buffer[CLIENT_RECEIVE_BUF_SIZE]){
    sockaddr_in sender_addr; // Client address
    socklen_t sender_len = sizeof(sender_addr);
    int bytes_received;

    while (true) {
        // Receive data from the client
        bytes_received = recvfrom(welcome_socket, buffer, sizeof(buffer) - 1, 0, (sockaddr*) &sender_addr, &sender_len);
        if (bytes_received <= 0) { // Error receiving data or client closed connection
            std::cout << "Error receiving data or client closed connection\n";
            continue;
        }
        std::string received_data; // Used to hold the received data
        // Null-terminate the received data
        buffer[bytes_received] = 0;
        // Append received data to a string
        received_data.append(buffer);
    }
}

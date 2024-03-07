#include "client_tcp.hpp"

#include "winsock_start.hpp"
#include <arpa/inet.h>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

void TCP_Client::connect() {
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(server_ip_);
    clientService.sin_port = htons(port_);

    if (connect(socket_, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        std::cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << "\n";
        WSACleanup();
        assert(0);
    } else {
        std::cout << "Client: Connect() is OK!\n";
        std::cout << "Client: Can start sending and receiving data...\n";
    }
}

void TCP_Client::send(char buffer[SEND_BUF_SIZE]) {
    int sbyteCount = send(socket_, buffer, strlen(buffer), 0);
    if(sbyteCount == SOCKET_ERROR) {
        std::cout << "Server send error: " << WSAGetLastError() << "\n";
    } else {
        std::cout << "Server: sent " << sbyteCount << " bytes\n";
    }
}

void TCP_Client::receive(char buffer[RECEIVE_BUF_SIZE]) {
    // Receive data from the client
    int bytes_received = recv(socket_, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) { // Error receiving data or client closed connection
        break;
    }
}

void TCP_Client::disconnect() {
    closesocket(socket_);
    WSACleanup();
}

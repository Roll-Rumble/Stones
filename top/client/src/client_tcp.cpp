#include "client_tcp.hpp"

#include "netutils.hpp"
#include "winsock_start.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

TCP_Client::TCP_Client(int port, std::string server_ip) {
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(server_ip);
    clientService.sin_port = htons(port);

    if (connect(socket_, (SOCKADDR*) &clientService, sizeof(clientService)) == SOCKET_ERROR) {
        WSACleanup();
        throw NetworkException;
    }
}

TCP_Client::~TCP_Client() {
    closesocket(socket_);
    WSACleanup();
}

void TCP_Client::send(char buffer[SEND_BUF_SIZE]) {
    int sbyteCount = send(socket_, buffer, strlen(buffer), 0);
    if(sbyteCount == SOCKET_ERROR) {
        throw NetworkException;
    }
}

void TCP_Client::receive(char buffer[RECEIVE_BUF_SIZE]) {
    // Receive data from the client
    int bytes_received = recv(socket_, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) { // Error receiving data or client closed connection
        throw NetworkException;
    }
}

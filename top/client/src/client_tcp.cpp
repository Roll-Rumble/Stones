#include "client_tcp.hpp"

#include "winsock_start.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET TCP_connect(std::string EC2_IP, int port, SOCKET &clientSocket) {
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(EC2_IP);
    clientService.sin_port = htons(port);
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
        cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    } else {
        cout << "Client: Connect() is OK!" << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }
    return clientSocket;
}

void TCP_send(SOCKET clientSocket, char buffer[200]) {
    int sbyteCount = send(clientSocket, buffer, strlen(buffer), 0);
    if(sbyteCount == SOCKET_ERROR){
        cout << "Server send error: " << WSAGetLastError() << endl;
        return -1;
    } else {
        cout << "Server: sent " << sbyteCount << " bytes" << endl;
    }
}

void TCP_receive(char buffer[1024]) {
    // Receive data from the client
    bytes_received = recv(connection_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) { // Error receiving data or client closed connection
        break;
    }
}

void TCP_disconnect(SOCKET clientSocket) {
    closesocket(clientSocket);
    WSACleanup();
}

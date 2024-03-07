#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <arpa/inet.h>
#include "winsock_start.hpp"
#include "client_udp.hpp"


void UDP_send(SOCKET clientSocket,sockaddr_in clientService, char buffer[1024]) { //data will be a pair of integers

    buffer = htons(buffer);

    int sbyteCount = sendto(clientSocket, &buffer, sizeof(buffer), 0, (SOCKADDR*)&clientService, sizeof(clientService));
    if(sbyteCount == SOCKET_ERROR){
        std::cout << "Server send error: " << WSAGetLastError() << std::endl;
        return;
    } else {
        std::cout << "Server: sent " << sbyteCount << " bytes" << std::endl;
    }
}

void UDP_receive(int PORT, const char* EC2_IP_ADDRESS, char buffer[1024]){
    sockaddr_in sender_addr; // Client address
    socklen_t sender_len = sizeof(sender_addr);
    int bytes_received;
    while (true) {
    // Receive data from the client
    bytes_received = recvfrom(welcome_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&sender_addr, &sender_len);
    if (bytes_received <= 0) { // Error receiving data or client closed connection
        std::cout << "Error receiving data or client closed connection" << std::endl;
        continue;
    }
    std::string received_data; // Used to hold the received data
    // Null-terminate the received data
    buffer[bytes_received] = 0;
    // Append received data to a string
    received_data.append(buffer);
    }
}

void UDP_setup(SOCKET &clientSocket){
    
    // WSAStartup
    WSAStartup();

    // Socket creation
    SOCKET senderSocket;
    SocketCreation(senderSocket); // Pass clientSocket by reference

    // Connection to server
    sockaddr_in senderService;
    senderService.sin_family = AF_INET;
    senderService.sin_port = htons(PORT); // Ensure the port is correct
    inet_pton(AF_INET, EC2_IP_ADDRESS, &senderService.sin_addr); // Ensure the IP is correct
    
    // Sending data
    send_data(clientSocket,senderService);

    // Properly close the socket
    closesocket(clientSocket);
    WSACleanup();
}
#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <arpa/inet.h>
#include <client/include/winsock_start.hpp>

#ifndef CLIENT_UDP_HPP
#define CLIENT_UDP_HPP


void UDP_send(SOCKET clientSocket,sockaddr_in clientService, std::pair<int16_t,int16_t> input_vals) { //data will be a pair of integers

    data_to_send data_being_sent;
    data_being_sent.x = htons(input_vals.first);
    data_being_sent.y = htons(input_vals.second);

    int sbyteCount = sendto(clientSocket, &data_being_sent, sizeof(data_being_sent), 0, (SOCKADDR*)&clientService, sizeof(clientService));
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

#endif

#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <arpa/inet.h>
#include <client/include/winsock_start.hpp>
    
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
#include "winsock_start.hpp"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

void WSAStartup() {
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if(wserr != 0){
        std::cout << "The winsock dll not found" << std::endl;
        return;
    } else {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }
}

void SocketCreation(SOCKET &clientSocket) { // Pass clientSocket by reference
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(clientSocket == INVALID_SOCKET){
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    } else {
        std::cout << "socket is OK!" << std::endl;
    }
}

#include "winsock_start.hpp"

#include "netutils.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

void start_WSA() {
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if (wserr) {
        throw NetworkException("winsock dll not found");
    }
}

void create_socket(SOCKET &clientSocket) { // Pass clientSocket by reference
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(clientSocket == INVALID_SOCKET){
        WSACleanup();
        throw NetworkException("error creating socket");
    }
}

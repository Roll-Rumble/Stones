#ifndef WINSOCK_START_HPP
#define WINSOCK_START_HPP

#include <arpa/inet.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

void WSAStartup();

void SocketCreation(SOCKET &clientSocket);

#endif

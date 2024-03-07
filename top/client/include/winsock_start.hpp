#ifndef WINSOCK_START_HPP
#define WINSOCK_START_HPP

#include <winsock2.h>

void WSAStartup();

void SocketCreation(SOCKET &clientSocket);

#endif

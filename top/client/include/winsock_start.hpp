#ifndef WINSOCK_START_HPP
#define WINSOCK_START_HPP

#include <winsock2.h>

void start_WSA();

void create_socket(SOCKET &clientSocket);

#endif

#ifndef WINSOCK_START_HPP
#define WINSOCK_START_HPP

#include <winsock2.h>

// TODO: MOVE THIS
#define SERVER_IP "13.49.68.13"

void start_WSA();

void create_socket(SOCKET &clientSocket);

#endif

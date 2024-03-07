#ifndef CLIENT_TCP_HPP
#define CLIENT_TCP_HPP

#include <string>
#include <winsock2.h>

SOCKET TCP_connect(std::string EC2_IP, int port, SOCKET &clientSocket);

void TCP_send(SOCKET clientSocket, char buffer[200]);

void TCP_receive(char buffer[1024]);

void TCP_disconnect(SOCKET clientSocket);

#endif

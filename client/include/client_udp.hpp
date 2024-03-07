#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <arpa/inet.h>

#ifndef CLIENT_UDP_HPP
#define CLIENT_UDP_HPP


void UDP_send(SOCKET clientSocket,sockaddr_in clientService, char buffer[1024]);

void UDP_receive(int PORT, const char* EC2_IP_ADDRESS, char buffer[1024]);

void UDP_setup();

#endif

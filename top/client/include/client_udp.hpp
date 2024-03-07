#ifndef CLIENT_UDP_HPP
#define CLIENT_UDP_HPP

#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <arpa/inet.h>

#define UDP_SEND_BUF_SIZE 1024
#define UDP_RECEIVE_BUF_SIZE 1024

class UDP_Client {
    public:
        UDP_Client(int port, std::string server_ip) :
                   port_(port), server_ip_(server_ip) {}
        ~UDP_Client() {}

        void send(char buffer[UDP_SEND_BUF_SIZE]);
        void receive(char buffer[UDP_RECEIVE_BUF_SIZE]);

    private:
        int port_;
        std::string server_ip_;
        SOCKET socket_;
        sockaddr_in client_service_;
};

#endif

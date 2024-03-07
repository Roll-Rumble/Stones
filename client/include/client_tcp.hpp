#ifndef CLIENT_TCP_HPP
#define CLIENT_TCP_HPP

#include <string>
#include <winsock2.h>

class TCP_Client {
public:
    TCP_Client(int port, std::string server_ip) :
               port(port), server_ip(server_ip) {}
    ~TCP_Client() {}

    SOCKET connect(std::string EC2_IP, int port, SOCKET &clientSocket);
    void send(SOCKET clientSocket, char buffer[200]);
    void receive(char buffer[1024]);
    void disconnect(SOCKET clientSocket);

private:
    int port;
    std::string server_ip;
    SOCKET socket;
};

#endif

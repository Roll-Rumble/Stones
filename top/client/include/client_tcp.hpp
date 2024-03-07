#ifndef CLIENT_TCP_HPP
#define CLIENT_TCP_HPP

#include <string>
#include <winsock2.h>

#define SEND_BUF_SIZE
#define RECEIVE_BUF_SIZE

class TCP_Client {
public:
    TCP_Client(int port, std::string server_ip) :
               port(port), server_ip(server_ip) {}
    ~TCP_Client() {}

    void connect();
    void send(char buffer[SEND_BUF_SIZE]);
    void receive(char buffer[RECEIVE_BUF_SIZE]);
    void disconnect();

private:
    int port_;
    std::string server_ip_;
    SOCKET socket_;
};

#endif

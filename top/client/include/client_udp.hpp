#ifndef CLIENT_UDP_HPP
#define CLIENT_UDP_HPP

#include <string>
#include <utility>
#include <winsock2.h>

#define UDP_SEND_BUF_SIZE 1024
#define UDP_RECEIVE_BUF_SIZE 1024

class UDP_Client {
public:
    UDP_Client(int port, std::string server_ip);
    ~UDP_Client(){};

    void send_xy(int16_t x, int16_t y);
    std::pair<float, float> receive_xy();

private:
    SOCKET socket_;
};

#endif

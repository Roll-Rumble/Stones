#ifndef CLIENT_UDP_HPP
#define CLIENT_UDP_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <winsock2.h>

// TODO: Move this later to netutils!!
#define CLIENT_UDP_RECV_PORT 12000
#define SERVER_UDP_PORT 12000

#define UDP_SEND_BUF_SIZE 1024
#define UDP_RECEIVE_BUF_SIZE 1024

class UDPClient {
public:
    UDPClient();
    ~UDPClient();

    void send_xy(int16_t x, int16_t y);
    std::pair<float, float> receive_xy(std::pair<float, float> def);

private:
    SOCKET send_socket_;
};

#endif

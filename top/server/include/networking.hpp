#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <utility>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <vector>

#define NUM_OUT_BYTES 8
#define NUM_IN_BYTES 4
#define LISTEN_PORT 12000
#define SEND_PORT 12000
#define TCP_LISTEN_PORT 13000

class TCPServ
{
public:
    TCPServ();
    ~TCPServ();

    std::vector<std::string> get_connections(int num_clients);
private:
    int sockfd_;
    std::vector<int> conn_socks_;
};

class UDPServ
{
public:
    UDPServ(std::string &addr);
    ~UDPServ();

    std::pair<int16_t, int16_t> recv_xy();
    void send_xy(float x, float y);
private:
    int sockfd_;
};



#endif
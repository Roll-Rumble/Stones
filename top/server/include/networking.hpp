#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <utility>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <cstring>

#define NUM_OUT_BYTES 8
#define NUM_IN_BYTES 4
#define LISTEN_PORT 12000
#define SEND_PORT 12000

std::pair<std::string, short> wait_for_tcp();


class UDPServ
{
private:
    int sockfd_;
public:
    UDPServ(std::string &addr);
    ~UDPServ();

    std::pair<int16_t, int16_t> recv_xy();
    void send_xy(float x, float y);
};

#endif
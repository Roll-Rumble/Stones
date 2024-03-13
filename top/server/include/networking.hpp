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

    std::vector<std::pair<std::string, int>> get_connections(int num_clients);
    std::pair<int16_t, int16_t> recv_xy(int client_id, std::pair<int16_t, int16_t> def);
    void send_xy(int client_id, float x, float y);

    void send_buffer(int client_id, unsigned char *buffer, size_t size);
    void recv_buffer(int client_id, unsigned char *buffer, size_t len);

    void send_int(int client_id, uint32_t val);
    uint32_t recv_int(int client_id);


private:
    int sockfd_;
    std::vector<int> conn_socks_;
};

class UDPServ
{
public:
    UDPServ(std::string &addr, uint32_t connection_nb);
    ~UDPServ();

    std::pair<int16_t, int16_t> recv_xy(std::pair<int16_t, int16_t> def);
    void send_xy(float x, float y);
private:
    int sockfd_;
};



#endif

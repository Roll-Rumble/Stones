#ifndef CLIENT_TCP_HPP
#define CLIENT_TCP_HPP

#include <string>
#include <vector>
#include <winsock2.h>
#include "game_util.hpp"


// TODO: MOVE THIS TO NETUTIL
#define SERVER_TCP_PORT 13000


#define SEND_BUF_SIZE 4
#define RECEIVE_BUF_SIZE 8

class TCPClient {
public:
    TCPClient();
    ~TCPClient();

    uint32_t get_connection_nb() const;
    void send_buffer(unsigned char* buffer, size_t len);
    void recv_buffer(unsigned char* buffer, size_t len);
    void send_int(uint32_t val);
    uint32_t recv_int();
    uint16_t recv_uint16();
    uint32_t get_nb_games();
    std::vector<std::vector<XYPairFloat>> get_game_data(int game_id);

    void send_xy(int16_t x, int16_t y);
    std::pair<float, float> receive_xy(std::pair<float, float> def);
private:
    SOCKET socket_;
    uint32_t connection_nb_;
};

#endif

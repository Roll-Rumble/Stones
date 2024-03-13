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
    void send_data(unsigned char buffer[SEND_BUF_SIZE]);
    void receive(unsigned char buffer[RECEIVE_BUF_SIZE]);
    void Convert_to_Buffers(const std::vector< std::vector<XYPairInt16> >& replay_data, char* data_buffer);
    int get_nb_games();

    void send_xy(int16_t x, int16_t y);
    std::pair<float, float> receive_xy(std::pair<float, float> def);
private:
    SOCKET socket_;
    uint32_t connection_nb_;
};

#endif

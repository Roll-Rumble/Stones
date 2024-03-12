#ifndef CLIENT_TCP_HPP
#define CLIENT_TCP_HPP

#include <string>
#include <winsock2.h>
#include <vector>


// TODO: MOVE THIS TO NETUTIL
#define SERVER_TCP_PORT 13000


#define SEND_BUF_SIZE 4
#define RECEIVE_BUF_SIZE 8

class TCPClient {
public:
    TCPClient();
    ~TCPClient();

    void send_data(unsigned char buffer[SEND_BUF_SIZE]);
    void receive(unsigned char buffer[RECEIVE_BUF_SIZE]);
    int Get_num_games();
    std::vector< std::vector<XYPairInt16> > Get_replay_data(uint32_t GameID);
private:
    SOCKET socket_;
};

#endif

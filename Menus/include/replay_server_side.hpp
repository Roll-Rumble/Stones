#ifndef REPLAY_CLIENT_SIDE_H
#define REPLAY_CLIENT_SIDE_H

#include <iostream>
#include <vector>
#include "client_tcp.hpp"
#include "winsock_start.hpp"
#include "game_util.hpp"
#include "netutils.hpp"

class ReplayServerSide : public TCP_Client {
public:
    
    void Send_nb_games();
    void Send_replay_data(uint32_t GameID);

private:
    SOCKET socket_;
    std::vector< std::vector<XYPairInt16> > replay_data_;
};

#endif
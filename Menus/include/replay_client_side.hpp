#ifndef REPLAY_CLIENT_SIDE_H
#define REPLAY_CLIENT_SIDE_H

#include <iostream>
#include <vector>
#include "client_tcp.hpp"
#include "winsock_start.hpp"
#include "game_util.hpp"
#include "netutils.hpp"

class ReplayClientSide : public TCP_Client {
public:
    
    int Get_num_games();
    std::vector< std::vector<XYPairInt16> > Get_replay_data(uint32_t GameID);

private:
    SOCKET socket_;
    std::vector< std::vector<XYPairInt16> > replay_data_;
};

#endif
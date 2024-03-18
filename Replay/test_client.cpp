#include "client_tcp.hpp"
#include "game_util.hpp"
#include <vector>
#include <iostream>

int main(){
    TCPClient client;
    int nb_games = client.get_nb_games();
    int GameID = 0;
    std::vector< std::vector<XYPairInt16> > replay_data2 = client.replay_fetch_protocal(GameID);
    
    for (int i = 0; i < replay_data2.size(); i++) {
        for (int j = 0; j < replay_data2[i].size(); j++) {
            std::cout << replay_data2[i][j].x << " " << replay_data2[i][j].y << std::endl;
        }
    }
}
#include "replay_client_side.hpp"

#define GAME_ID_SIZE 4
#define NB_GAMES_SIZE 4
#define REPLAY_SIZE_SIZE 4
#define NB_BALLS 2

int ReplayClientSide::Get_num_games(){
    int num_games = 0;
    unsigned char num_games_buffer[NB_GAMES_SIZE];
    receive(num_games_buffer); //receive function will be changed to unsigned
    num_games = pack::unpacku32(num_games_buffer);
    return num_games;
}

std::vector< std::vector<XYPairInt16> > ReplayClientSide::Get_replay_data(uint32_t GameID){
    unsigned char num_of_frames_buffer[REPLAY_SIZE_SIZE];
    unsigned char game_id_buffer[GAME_ID_SIZE];
    
    send_data((char*)&GameID);
    
    pack::packu32(game_id_buffer, GameID); 
    send_data((char*)&game_id_buffer);

    receive(num_of_frames_buffer); //receive function will be changed to unsigned
    int num_of_frames = pack::unpacku32(num_of_frames_buffer);

    unsigned char replay_data_buffer[num_of_frames];

    std::vector< std::vector<XYPairInt16> > game_replay_data;

    for(int i = 0; i < num_of_frames; i++){
        receive(replay_data_buffer); //receive function will be changed to unsigned
        game_replay_data.push_back(decode_input(replay_data_buffer));
    }

    return game_replay_data;
}
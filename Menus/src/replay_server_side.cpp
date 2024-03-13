#include "replay_server_side.hpp"
#include "replay_packing.cpp"
#include "db.hpp"

#define GAME_ID_SIZE 4
#define NB_GAMES_SIZE 4
#define REPLAY_SIZE_SIZE 4

void ReplayServerSide::Send_nb_games(){
    uint32_t num_games = 0;
    Logger db(0);
    num_games = db.GetLatestGame();
    unsigned char num_games_buffer[NB_GAMES_SIZE];
    pack::packu32(num_games_buffer, num_games);
    send_data(num_games_buffer);

}

void ReplayServerSide::Send_replay_data(uint32_t GameID){
    Logger db(GameID);
    
    std::vector< std::vector<XYPairInt16> > replay_data = db.Parse(GameID);
    uint32_t num_frames = replay_data.size();
    unsigned char num_frames_buffer[REPLAY_SIZE_SIZE];
    pack::packu32(num_frames_buffer, num_frames);
    send_data(num_frames_buffer);
    unsigned char replay_data_buffer[8 * num_frames];
    for (int i = 0; i < num_frames; i++){
        encode_input(replay_data_buffer, replay_data[i][0].x, replay_data[i][0].y);
        encode_input(replay_data_buffer, replay_data[i][1].x, replay_data[i][1].y);
    }
    send_data(replay_data_buffer);
    
}
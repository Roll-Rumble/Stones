#include "client_tcp.hpp"

#include "netutils.hpp"
#include "winsock_start.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

TCPClient::TCPClient() {
    start_WSA();

    try {
        ADDRINFOA *client_addr_info = net::addr_info(
            SERVER_IP, SERVER_TCP_PORT, SOCK_STREAM);
        
        socket_ = socket(client_addr_info->ai_family,
            client_addr_info->ai_socktype, client_addr_info->ai_protocol);
        if (socket_ == SOCKET_ERROR) {
            throw NetworkException("can't create socket");
        }

        int reuse = 1;
        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
            (char *)&reuse, sizeof(int)) == SOCKET_ERROR) {
                throw NetworkException("can't set UDP socket to be reusable");
        }


        if (connect(socket_, client_addr_info->ai_addr,
            client_addr_info->ai_addrlen) == SOCKET_ERROR) {
            throw NetworkException("can't connect socket");
        }

        freeaddrinfo(client_addr_info);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }

}

TCPClient::~TCPClient() {
    closesocket(socket_);
    WSACleanup();
}

void TCPClient::send_data(unsigned char buffer[SEND_BUF_SIZE]) {
    try {
        net::send_buf(socket_, buffer, SEND_BUF_SIZE);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
}

void TCPClient::receive(unsigned char buffer[RECEIVE_BUF_SIZE]) {
    try {
        memset(buffer, 0, RECEIVE_BUF_SIZE);
        net::recv_buf(socket_, buffer, RECEIVE_BUF_SIZE);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
}

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

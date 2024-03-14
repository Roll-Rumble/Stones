#include <sys/types.h>
#include <sys/socket.h> // linux stuff
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <utility>
#include <iostream>

#include "db.hpp"
#include "netutils.hpp"
#include "networking.hpp"


TCPServ::TCPServ()
{
    ADDRINFOA *server_addr_info = net::addr_info(
        "0.0.0.0", TCP_LISTEN_PORT, SOCK_STREAM);

    sockfd_ = socket(server_addr_info->ai_family,
        server_addr_info->ai_socktype, server_addr_info->ai_protocol);
    if (sockfd_ == -1) {
        throw NetworkException("can't create socket");
    }

    int reuse = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            throw NetworkException("can't set UDP socket to be reusable");
    }


    if (bind(sockfd_, server_addr_info->ai_addr,
        server_addr_info->ai_addrlen) == -1) {
        throw NetworkException("can't bind socket");
    }

    freeaddrinfo(server_addr_info);
}

TCPServ::~TCPServ()
{
    std::cout << "Calling TCP socket destructor\n";
    close(sockfd_);
    for (int sock : conn_socks_) {
        close(sock);
    }
}

std::vector<std::pair<std::string, int>> TCPServ::get_connections(int num_clients)
{
    if (listen(sockfd_, num_clients) == -1) {
        throw NetworkException("can't listen for tcp connection");
    }

    std::vector<std::pair<std::string, int>> out;

    sockaddr_storage client_addr; // Client address
    socklen_t client_len = sizeof(client_addr);

    for (int i = 0; i < num_clients; i++) {
        conn_socks_.push_back(accept(sockfd_, (sockaddr *)(&client_addr), &client_len));
        if (conn_socks_[conn_socks_.size()-1] == -1) {
            throw NetworkException("can't accept connection");
        }
        out.push_back(net::get_addr_and_port(&client_addr));
    }

	return out;
}

void TCPServ::send_xy(int client_id, float x, float y)
{
    unsigned char buf[NUM_OUT_BYTES];
    pack::encode_pos(buf, x, y);

    net::send_buf(conn_socks_[client_id], buf, NUM_OUT_BYTES);
}

std::pair<int16_t, int16_t> TCPServ::recv_xy(int client_id, std::pair<int16_t, int16_t> def)
{
    unsigned char buf[4];
    if (net::recv_buf(conn_socks_[client_id], buf, 4, -1)) {
        return pack::decode_input(buf);
    } else {
        return def;
    }
}

void TCPServ::send_buffer(int client_id, unsigned char *buffer, size_t size)
{
    net::send_buf(conn_socks_[client_id], buffer, size);
}

void TCPServ::recv_buffer(int client_id, unsigned char *buffer, size_t len)
{
    net::recv_buf(conn_socks_[client_id], buffer, len, -1);
}

void TCPServ::send_int(int client_id, uint32_t val)
{
    unsigned char buf[sizeof(uint32_t)];
    pack::packu32(buf, val);

    net::send_buf(conn_socks_[client_id], buf, sizeof(uint32_t));
}

uint32_t TCPServ::recv_int(int client_id)
{
    unsigned char buf[sizeof(uint32_t)];
    net::recv_buf(conn_socks_[client_id], buf, sizeof(uint32_t), -1);
    return pack::unpacku32(buf);
}


// returns socket file descriptor
UDPServ::UDPServ(std::string &addr, uint32_t connection_nb)
{
    std::cout << "Creating UDP to send to address " << addr << " port " << SEND_PORT << "\n";
    ADDRINFOA *client_addr_info = net::addr_info(
        addr, SEND_PORT, SOCK_DGRAM);

    ADDRINFOA *server_addr_info = net::addr_info(
        "0.0.0.0", LISTEN_PORT + connection_nb, SOCK_DGRAM);

    sockfd_ = socket(client_addr_info->ai_family,
        client_addr_info->ai_socktype, client_addr_info->ai_protocol);
    if (sockfd_ == -1) {
        throw NetworkException("can't create socket");
    }

    int reuse = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1) {
            throw NetworkException("can't set UDP socket to be reusable");
    }

    std::cout << "Binding socket to port " << LISTEN_PORT + connection_nb << "\n";
    if (bind(sockfd_, server_addr_info->ai_addr,
        server_addr_info->ai_addrlen) == -1) {
        throw NetworkException("can't bind socket");
    }

    if (connect(sockfd_, client_addr_info->ai_addr,
        client_addr_info->ai_addrlen) == -1) {
        throw NetworkException("can't connect to socket");
    }
    std::cout << "Created socket " << sockfd_ << "\n";

    freeaddrinfo(client_addr_info);
    freeaddrinfo(server_addr_info);
}

UDPServ::~UDPServ()
{
    std::cout << "Calling UDP socket destructor on socket " << sockfd_ << "\n";
    close(sockfd_);
}


void UDPServ::send_xy(float x, float y)
{
    unsigned char buf[NUM_OUT_BYTES];
    pack::encode_pos(buf, x, y);
    net::send_buf(sockfd_, buf, NUM_OUT_BYTES);
}

std::pair<int16_t, int16_t> UDPServ::recv_xy(std::pair<int16_t, int16_t> def)
{
    unsigned char buf[4];
    if (net::recv_buf(sockfd_, buf, 4, 1000)) {
        return pack::decode_input(buf);
    } else {
        return def;
    }
}



// void TCPServ::send_nb_games(){
//     uint32_t num_games;
//     Logger db(0);
//     num_games = db.GetLatestGame();
//     unsigned char num_games_buffer[NB_GAMES_SIZE];
//     pack::packu32(num_games_buffer, num_games);
//     send_data(num_games_buffer);
// }

// void TCPServ::send_data(const char *data_buffer){
//     send(sockfd_, data_buffer, BUFFER_SIZE, 0);
// }

// void TCPServ::recieve_data(char *data_buffer, int buffer_size){
//     recv(sockfd_, data_buffer, buffer_size, 0);
// }

// int TCPServ::recieve_game_ID(){
//     //recieve game ID from client
//     char *game_ID_[GAME_ID_SIZE];
//     recieve_data(game_ID_, GAME_ID_SIZE); // not sure about this buffer
//     return game_ID_[3]
// }

// void TCPServ::send_replay_data(uint32_t GameID){
//     Logger db(GameID);
//     std::vector< std::vector<XYPairInt16> > replay_data = db.Parse(GameID);
//     Convert_to_Buffers(replay_data);
// }

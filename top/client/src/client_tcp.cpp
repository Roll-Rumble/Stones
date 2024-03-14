#include "client_tcp.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#define REPLAY_SIZE_SIZE 4
#define GAME_ID_SIZE 4


#include "netutils.hpp"
#include "winsock_start.hpp"


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

    // Get connection number
    unsigned char buffer[RECEIVE_BUF_SIZE];
    recv_buffer(buffer, RECEIVE_BUF_SIZE);
    connection_nb_ = (unsigned int) pack::decode_pos(buffer).first;
    std::cout << "Connected with connection number " << (int) connection_nb_ << "\n";
}

TCPClient::~TCPClient() {
    closesocket(socket_);
    WSACleanup();
}

uint32_t TCPClient::get_connection_nb() const
{
    return connection_nb_;
}

void TCPClient::send_buffer(unsigned char *buffer, size_t len) {
    try {
        net::send_buf(socket_, buffer, len);
    } catch (std::exception &e) {
        WSACleanup();
        throw e;
    }
}

void TCPClient::recv_buffer(unsigned char *buffer, size_t len) {
    try {
        net::recv_buf(socket_, buffer, len, -1);
    } catch (std::exception &e) {
        std::cout << "WSA error: " << WSAGetLastError() << "\n";
        WSACleanup();
        throw e;
    }
}

void TCPClient::send_int(uint32_t val)
{
    unsigned char buf[sizeof(uint32_t)];
    pack::packu32(buf, val);

    send_buffer(buf, sizeof(uint32_t));
}

uint32_t TCPClient::recv_int()
{
    unsigned char buf[sizeof(uint32_t)];
    recv_buffer(buf, sizeof(uint32_t));
    return pack::unpacku32(buf);
}

uint16_t TCPClient::recv_uint16()
{
    unsigned char buf[sizeof(uint16_t)];
    recv_buffer(buf, sizeof(uint16_t));
    return pack::unpacku16(buf);
}

void TCPClient::send_xy(int16_t x, int16_t y) {
    unsigned char buffer[4];
    pack::encode_input(buffer, x, y);

    try {
        net::send_buf(socket_, buffer, sizeof(buffer));
    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << "\n";
        wchar_t* s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&s, 0, NULL);
        fprintf(stderr, "%S\n", s);
        LocalFree(s);
        WSACleanup();
        throw e;
    }
}

std::pair<float, float> TCPClient::receive_xy(std::pair<float, float> def) {
    unsigned char buffer[8];
    try {
        // TCP recv has timeout 1000s
        if (net::recv_buf(socket_, buffer, sizeof(buffer), 1000000000)) {
            return pack::decode_pos(buffer);
        } else {
            return def;
        }

    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << "\n";
        WSACleanup();
        throw e;
    }
}
uint32_t TCPClient::get_nb_games()
{
    char buf[1];
	buf[0] = 'g';
    send_buffer((unsigned char*) buf, 1);

    return recv_int();
}

std::vector<std::vector<XYPairFloat>> TCPClient::get_game_data(int game_id)
{
    char buf_com = 'd';
    send_buffer((unsigned char*)&buf_com, 1);
    send_int(game_id);
    uint32_t game_size = recv_int();
    std::cout << "Received game size: " << game_size << std::endl;
    std::vector<std::vector<XYPairFloat>> out;
    out.resize(game_size);
    for (int i = 0; i < game_size; i++) {
        out[i].resize(2);
        out[i][0] = {.x = static_cast<float>(recv_uint16()),
            .y = static_cast<float>(recv_uint16())};
        out[i][1] = {.x = static_cast<float>(recv_uint16()),
            .y = static_cast<float>(recv_uint16())};
    }
    return out;
}

// std::vector< std::vector<XYPairInt16> > replay_fetch_protocol(uint32_t GameID) {
//     char game_id_buffer[GAME_ID_SIZE]; // buffer that holds the game id to send to server to request replay
//     char replay_data_buffer[1024]; // buffer that holds the replay data received from server
// std::vector< std::vector<XYPairInt16> > TCPClient::replay_fetch_protocal(uint32_t GameID) {
//     unsigned char game_id_buffer[GAME_ID_SIZE]; // buffer that holds the game id to send to server to request replay
//     unsigned char replay_data_buffer[1024]; // buffer that holds the replay data received from server
//     std::vector< std::vector<XYPairInt16> > game_replay_tmp;
//     std::vector< std::vector<XYPairInt16> > game_replay_final;
//     TCPClient client;

//     pack::packu32(game_id_buffer, GameID);
//     client.send_data(game_id_buffer); // send game id buffer

//     bool finished = false;
//     while(!finished){
//         client.receive(replay_data_buffer); //receive function will be changed to unsigned
//         game_replay_tmp = Convert_to_vector(replay_data_buffer);
//         game_replay_final.insert(game_replay_final.end(), game_replay_tmp.begin(), game_replay_tmp.end());


//         if(game_replay_tmp[game_replay_tmp.size() - 1][0].x == 33333){ // means we have recieved all data and are done.
//             finished = true;
//         }

//     }

//     //game_replay_data.insert(game_replay_data.end(), decode_input(replay_data_buffer)
//     // similar to the above line, but for all the frames
//     return game_replay_final;
// }

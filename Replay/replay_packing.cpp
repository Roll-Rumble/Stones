#include <iostream>
#include <vector>
#include "game_util.hpp"
#include "client_tcp.hpp"

#define BUFFER_SIZE 1024
#define INT16_SIZE 2

void pack32(char *buf, uint32_t xy) {
    buf[0] = (xy >> 24) & 0xFF; //  we AND with 0xFF to ensure that the value is 8 only bits long (= 1 byte)
    buf[1] = (xy >> 16) & 0xFF;
    buf[2] = (xy >> 8) & 0xFF;
    buf[3] = xy & 0xFF;
}


void encode_input(char *buf, int16_t x, int16_t y) {
    uint32_t xy = ((uint32_t)x << 16) + (uint32_t)y; // x is most significant 16 bits and y is least significant 16 bits
    pack32(buf, xy);
}


void Convert_to_Buffers(const std::vector< std::vector<XYPairInt16> > &replay_data) {
    char data_buffer[BUFFER_SIZE];
    unsigned int buffer_index = 0;
    TCP_Client TCPclient;

    for (const auto frame : replay_data) {
        for (const auto xy_pair : frame) { // each frame has 2 pairs, one for each ball
            encode_input(data_buffer, xy_pair.x, xy_pair.y);
            buffer_index += 2 * INT16_SIZE; // each pair has 2 int16, x and y
            if (buffer_index >= BUFFER_SIZE) {
                TCPclient.send_data(data_buffer);
                buffer_index = 0;
            }
        }
    }
    if (buffer_index > 0) {
        TCPclient.send_data(data_buffer);
    }
}


/*
from vector of vectors of XYPair as parameter convert into some buffers all length 1024 bytes

see packing function in top/netutils.cpp

[[frame1] //each vextor holds 8 bytes
[frame2]
.
.
.
[last frame, frame n]]
*/
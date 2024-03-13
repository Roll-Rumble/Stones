#include <iostream>
#include <vector>
#include "game_util.hpp"
#include "client_tcp.hpp"

#define BUFFER_SIZE 32
#define INT16_SIZE 2

void pack32(char *buf, uint32_t xy) {
    *buf++ = xy>>24; *buf++ = xy>>16;
    *buf++ = xy>>8;  *buf++ = xy;
}


void encode_input(char *buffer, int16_t x, int16_t y) {
    uint32_t xy = ((uint32_t)x << 16) + (uint32_t)y; // x is most significant 16 bits and y is least significant 16 bits
    pack32(buffer, xy);
}


// void Convert_to_Buffers(const std::vector< std::vector<XYPairInt16> > &replay_data) {
//     char data_buffer[BUFFER_SIZE];
//     unsigned int buffer_index = 0;
//     TCP_Client TCPclient;

//     for (const auto& frame : replay_data) {
//         for (const auto& xy_pair : frame) { // each frame has 2 pairs, one for each ball
//             encode_input(data_buffer, xy_pair.x, xy_pair.y);
//             buffer_index += 2 * INT16_SIZE; // each pair has 2 int16, x and y
//             if (buffer_index >= BUFFER_SIZE) {
//                 TCPclient.send_data(data_buffer);
//                 buffer_index = 0;
//             }
//         }
//     }
//     if (buffer_index > 0) {
//         TCPclient.send_data(data_buffer);
//     }
// }


void Convert_to_Buffers(const std::vector< std::vector<XYPairInt16> >& replay_data, char* data_buffer) {
    unsigned int buffer_index = 0;

    for (const std::vector<XYPairInt16>& frame : replay_data) {
        int i = 0;
        for (const XYPairInt16& xy_pair : frame) { // each frame has 2 pairs, one for each ball
            encode_input(data_buffer, xy_pair.x, xy_pair.y);
            data_buffer += 2 * INT16_SIZE;
            buffer_index += 2 * INT16_SIZE; // each pair has 2 int16, x and y
            if (buffer_index >= BUFFER_SIZE) {
                // If buffer is full, stop processing
                return;
            }
        }
        i = 0;
    }
}


XYPairInt16 toXYpair(int16_t x, int16_t y) {
    XYPairInt16 xy;
    xy.x = x;
    xy.y = y;
    return xy;
}


std::vector< std::vector<XYPairInt16> > Convert_to_vector (const char *data_buffer) {
    std::vector< std::vector<XYPairInt16> > replay_data;
    std::vector<XYPairInt16> frame;

    unsigned int buffer_index = 0;

    while (buffer_index < BUFFER_SIZE) {
        int16_t x = (data_buffer[buffer_index++] << 8) + data_buffer[buffer_index++]; // for ball 0
        int16_t y = (data_buffer[buffer_index++] << 8) + data_buffer[buffer_index++];
        frame.push_back(toXYpair(x, y));

        x = (data_buffer[buffer_index++] << 8) + data_buffer[buffer_index++]; // for ball 1
        y = (data_buffer[buffer_index++] << 8) + data_buffer[buffer_index++];
        frame.push_back(toXYpair(x, y));

        replay_data.push_back(frame);
        frame.clear();
    }
    return replay_data;
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

from buffer to vector just do opposite 
*/
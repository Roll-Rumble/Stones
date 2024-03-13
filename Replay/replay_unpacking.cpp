#include <iostream>
#include <vector>
#include "game_util.hpp"
#include "client_tcp.hpp"

#define BUFFER_SIZE 1024
#define INT16_SIZE 2

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
        int16_t x = data_buffer[buffer_index++] << 8 + data_buffer[buffer_index++]; // for ball 0
        int16_t y = data_buffer[buffer_index++] << 8 + data_buffer[buffer_index++];
        frame.push_back(toXYpair(x, y));

        x = data_buffer[buffer_index++] << 8 + data_buffer[buffer_index++]; // for ball 1
        y = data_buffer[buffer_index++] << 8 + data_buffer[buffer_index++];
        frame.push_back(toXYpair(x, y));
        
        replay_data.push_back(frame);
        frame.clear();
    }
    return replay_data;
}


/*
do opposite, from buffers reconstruct te vector of vectors of XYPair
*/
#include <iostream>
#include "replay_packing.cpp"

#define BUFFER_SIZE 32

int main() {
    std::vector< std::vector<XYPairInt16> > replay_data;
    std::vector<XYPairInt16> frame;
    int16_t x = 1;
    int16_t y = 2;
    char buffer[BUFFER_SIZE];

    frame.push_back(XYPairInt16(x, x)); // 1 1
    frame.push_back(XYPairInt16(x, y)); // 1 2
    replay_data.push_back(frame);
    frame.clear();

    frame.push_back(XYPairInt16(y, x)); // 2 1
    frame.push_back(XYPairInt16(y, y)); // 2 2
    replay_data.push_back(frame);
    frame.clear();

    frame.push_back(XYPairInt16(y, x)); // 2 1
    frame.push_back(XYPairInt16(y, y)); // 2 2
    replay_data.push_back(frame);
    frame.clear();

    frame.push_back(XYPairInt16(y, x)); // 2 1
    frame.push_back(XYPairInt16(y, y)); // 2 2
    replay_data.push_back(frame);

    Convert_to_Buffers(replay_data, buffer);
    
    std::vector< std::vector<XYPairInt16> > replay_data2 = Convert_to_vector(buffer);
    for (int i = 0; i < replay_data2.size(); i++) {
        for (int j = 0; j < replay_data2[i].size(); j++) {
            std::cout << replay_data2[i][j].x << " " << replay_data2[i][j].y << std::endl;
        }
    }

    return 0;
}
#include "db.hpp"
#include <string>

#define GAMEID 0
#define BUFFER_SIZE 1024
#define INT16_SIZE 2

void pack32(char *buf, uint32_t xy) {
    *buf++ = xy>>24; *buf++ = xy>>16;
    *buf++ = xy>>8;  *buf++ = xy;
}


void encode_input(char *buffer, int16_t x, int16_t y) {
    uint32_t xy = ((uint32_t)x << 16) + (uint32_t)y; // x is most significant 16 bits and y is least significant 16 bits
    pack32(buffer, xy);
}

void Convert_to_Buffers(const std::vector< std::vector<XYPairInt16> >& replay_data, std::vector<char*>) { //return a vectors of all the pointers
    unsigned int buffer_index = 0;
	double size = replay_data.size();
	double num_buffers = size/128;
	int index = 0;

    for (const std::vector<XYPairInt16>& frame : replay_data) {
		for (const XYPairInt16& xy_pair : frame) { // each frame has 2 pairs, one for each ball
            encode_input(data_buffer, xy_pair.x, xy_pair.y);
            data_buffer += 2 * INT16_SIZE;
            buffer_index += 2 * INT16_SIZE; // each pair has 2 int16, x and y
            if (buffer_index >= BUFFER_SIZE) {
                // If buffer is full, stop processing
                return;
            }
        }
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

/*int main(){ // old tester
	Logger log("Storage.txt");


	int FrameID = 0;
	std::vector <std::pair<std::string, std::string> > input;
	std::vector <std::pair<std::string, std::string> > output;
	input.push_back( std::make_pair("0", "0"));
	input.push_back( std::make_pair("0", "0"));
	//std::cout << test.substr(test.find(":") + 1, 1);
	log.Put(input, FrameID);
	for(int i =1; i < 100; i++ ){
		input[0].first = std::to_string(i);
		input[1].second = std::to_string(i);
		log.Put(input, i);
	}
	//Put(input, 3, FrameID);
	//for(int i =1; i < 100; i++ ){
	//	Put(input, 3, FrameID);
	//}
	output = log.Get(65);
	std::cout << output[0].first << " " << output[0].second << std::endl;
	std::cout << output[1].first << " " << output[1].second << std::endl;
	output = log.Get(3);
	std::cout << output[0].first << " " << output[0].second << std::endl;
	std::cout << output[1].first << " " << output[1].second << std::endl;

}*/

int main() {
	int FrameID = 0;
	char buffer[BUFFER_SIZE];

	Logger log(GAMEID);
	std::vector <std::pair<std::string, std::string> > input;
	std::vector<std::vector <XYPairInt16 > > output;
	input.push_back( std::make_pair("0", "0"));
	input.push_back( std::make_pair("0", "0"));
	//std::cout << test.substr(test.find(":") + 1, 1);
	log.Put(input);
	std::cout << "put" << std::endl;
	for(int i =1; i < 1000; i++ ){
		input[0].first = std::to_string(i);
		input[1].second = std::to_string(i);
		log.Put(input);
	}
	log.Close();

	output = log.Parse(0);
	std::cout << "parse" << std::endl;
	std::cout << output[0][0].x << " " << output[0][0].y << std::endl;
	std::cout << output[1][1].x << " " << output[1][1].y << std::endl;
	std::fstream file("Storage0.txt", std::ios::app);
	for(int i = 0; i < output.size(); i++){
		file << "Frame " << i << std::endl;
		for(int j = 0; j < output[i].size(); j++){
			file << output[i][j].x << " " << output[i][j].y << std::endl;
		}
		file << std::endl;
	}
	std::cout << "closed" << std::endl;
	//output = log.Get(3,GAMEID);
	//std::cout << output[0].first << " " << output[0].second << std::endl;
	//std::cout << output[1].first << " " << output[1].second << std::endl;
	//log.Open("Storage0.json");

	// g++ -std=c++17 main.cpp db.cpp -o main   // compile using C++ 17 or later
	Convert_to_Buffers(output, buffer);
    
    std::vector< std::vector<XYPairInt16> > replay_data2 = Convert_to_vector(buffer);
    for (int i = 0; i < replay_data2.size(); i++) {
        for (int j = 0; j < replay_data2[i].size(); j++) {
            std::cout << replay_data2[i][j].x << " " << replay_data2[i][j].y << std::endl;
        }
    }
}


#include "db.hpp"
#include <string>
#define GAMEID 0

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
	Logger log(GAMEID);
	std::vector <std::pair<std::string, std::string> > input;
	std::vector<std::vector <XYPairInt16 > > output;
	input.push_back( std::make_pair("0", "0"));
	input.push_back( std::make_pair("0", "0"));
	//std::cout << test.substr(test.find(":") + 1, 1);
	log.Put(input);
	for(int i =1; i < 100; i++ ){
		input[0].first = std::to_string(i);
		input[1].second = std::to_string(i);
		log.Put(input);
	}
	log.Close();

	output = log.Parse(0);
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
	//output = log.Get(3,GAMEID);
	//std::cout << output[0].first << " " << output[0].second << std::endl;
	//std::cout << output[1].first << " " << output[1].second << std::endl;
	//log.Open("Storage0.json");
}
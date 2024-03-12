#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "db.hpp"
// for the first call FrameId must be 0
//using namespace std;
// json file_ has format:
//name of file_ is gameID
//Ball Id : x : y

int Logger::Put(std::vector<std::pair<std::string, std::string> > input, int &FrameID) {
	
	//std::ofstream file_("Storage.txt", std::ios::app); // later use game id

	
	
	if (file_.is_open()) {
		
		if (FrameID == 0) {
		
		file_ << "{" << std::endl;
		file_ << " \"BallNo\":" << input.size() << "," <<std::endl << " \"data\": [" << std::endl;
		}

		//file_ << "FrameID:" << FrameID << std::endl;
		if (FrameID != 0) {
			file_ <<"," << std::endl;
		}
		file_ << "[ ";
		  
		for (int i = 0; i < input.size(); i++){
			file_ << "[" << i << "," << input[i].first << "," << input[i].second << "]";
			if (i != input.size() - 1) {
				file_ << ", ";
			}
		}
		file_ << "]";
		//file_.close();
		//std::cout << "Put is successful" << std::endl;
		return 0;	
	}
		
	return 1;
}

std::vector<std::vector<XYPairInt16> > Logger::Parse(int GameID) {
	std::vector<std::vector<XYPairInt16> > output;
	std::vector<XYPairInt16 > temp;
	XYPairInt16 pair;
	std::string filename = "Storage" + std::to_string(GameID) + ".json";
	std::ifstream file(filename);
	std::string line;
	//std::string Frame =std::to_string(FrameID);
	std::string x;
	std::string y;
	int start;
	int comma1;
	int comma2;
	bool found = false;
	if (file.is_open()) {
		std::getline(file, line);
		std::getline(file, line);
		int BallNo = stoi(line.substr(line.find(":") +1));
		std::getline(file, line);
		int index = 0;
		std::getline(file, line);
		while (line[line.length() -1] != '}') {
			start = line.find("[ [");
			if ( start != std::string::npos) {
				for (int i = 0; i < BallNo; i++) {
					
					//if(line.substr(start + 3, 1) == std::to_string(i)){
						comma1 = line.find(",", start + 3);
						comma2 = line.find(",", comma1 + 1);
						x = line.substr(comma1 + 1, comma2 - comma1 - 1);
						y = line.substr(comma2 + 1, line.find("]") - comma2 - 1);
					//}
					pair.x = stoi(x);
					pair.y = stoi(y);
					//std::cout << "test" << std::endl;
					temp.push_back(pair);
					//std::cout << "test2" << std::endl;
					line = line.substr(line.find("]") + 2);
				}
				std::getline(file, line);
				
				output.push_back(temp);
				temp.clear();
				//std::cout << "output generated" << std::endl;
			}
		}
		file.close();
		//std::cout << "Get is successful" << std::endl;
		return output;
	}
	//std::cout << "Get is unsuccessful" << std::endl;
	throw "File not found";
	}
	

/*int main() {
	//std::string test = "123:3:5";
	//std::string output;
	int FrameID = 0;
	std::vector <std::pair<std::string, std::string> > input;
	std::vector <std::pair<std::string, std::string> > output;
	input.push_back( std::make_pair("1", "2"));
	input.push_back( std::make_pair("3", "4"));
	//std::cout << test.substr(test.find(":") + 1, 1);
	Put(input, 0, FrameID);
	for(int i =1; i < 100; i++ ){
		Put(input, 0, i);
	}
	//Put(input, 3, FrameID);
	//for(int i =1; i < 100; i++ ){
	//	Put(input, 3, FrameID);
	//}
	output = Get(0, 0);
	std::cout << output[0].first << " " << output[0].second << std::endl;
	std::cout << output[1].first << " " << output[1].second << std::endl;

}*/

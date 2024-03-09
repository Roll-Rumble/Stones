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
		file_ << "BallNo:" << input.size() << std::endl;
		}
		file_ << "FrameID:" << FrameID << std::endl;
		for (int i = 0; i < input.size(); i++){
			file_ << i+1 << ":" << input[i].first << ":" << input[i].second << std::endl;
		}
		//file_.close();
		//std::cout << "Put is successful" << std::endl;
		return 0;	
	}
		
	return 1;
}

std::vector<std::pair< std::string, std::string> > Logger::Get( int FrameID) {
	std::vector<std::pair<std::string, std::string> > output;
	
	std::ifstream file("Storage.txt");
	std::string line;
	std::string Frame = "FrameID:" + std::to_string(FrameID);
	std::string x;
	std::string y;
	int colon1;
	int colon2;
	bool found = false;
	if (file.is_open()) {
		std::getline(file, line);
		int BallNo = stoi(line.substr(line.find(":") +1));
		while (std::getline(file, line)) {

			if (line.find(Frame) != std::string::npos) {
				for (int i = 0; i < BallNo; i++) {
					std::getline(file, line);
					colon1 = line.find(":") + 1;
					colon2 = line.find(":", colon1 );

					x = line.substr(colon1 ,colon2 - colon1); 
					y = line.substr(colon2 + 1);
					output.push_back(std::make_pair(x, y));
				}
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

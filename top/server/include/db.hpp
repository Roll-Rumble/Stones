#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "game_util.hpp"
#include <filesystem>

class Logger{

public:
	Logger(const int GameID);
	~Logger(){
		file_ << std::endl << "]}";
		file_.close();
	};
	int Put(std::vector<std::pair<std::string, std::string> > input);
	std::vector<std::vector< XYPairInt16 > > Parse(int GameID);
	void Close() {
		file_ << std::endl << "]}";
		file_.close();
	};
	void Open(const std::string &file){
		file_.open(file, std::ios::app);
	};
	uint32_t GetLatestGame(){
		return latest_game_;
	};
	
	
private:
	std::ofstream file_;
	uint32_t latest_game_;
	int gameID_;
	int game_length_;
	int frame_ID_;
	

};
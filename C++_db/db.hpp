#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "top/game_objects/include/game_util.hpp"

class Logger{

public:
	Logger(const int &GameID){
		gameID_ = GameID;
		std::string filename = "Storage" + std::to_string(GameID) + ".json";
		file_.open(filename, std::ios::app);
	};
	~Logger(){
		file_ << std::endl << "]}";
		file_.close();
	};
	int Put(std::vector<std::pair<std::string, std::string> > input, int &FrameID);
	static std::vector<std::vector< XYPairInt16 > > Parse(int GameID);
	void Close(){
		file_ << std::endl << "]}";
		file_.close();
	};
	void Open(const std::string &file){
		file_.open(file, std::ios::app);
	};
	int gameID_;
private:
	std::ofstream file_;
	

};
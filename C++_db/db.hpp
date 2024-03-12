#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "top/game_objects/include/game_util.hpp"
#include <filesystem>

class Logger{

public:
	Logger(const int &GameID){
		gameID_ = GameID;
		std::string filename = "Replays/Storage" + std::to_string(GameID) + ".json";
		file_.open(filename, std::ios::app);
		latest_game_ = std::distance(std::filesystem::directory_iterator("Replays"), std::filesystem::directory_iterator()) - 1;
		

	};
	~Logger(){
		file_ << std::endl << "]}";
		file_.close();
	};
	int Put(std::vector<std::pair<std::string, std::string> > input, int &FrameID);
	std::vector<std::vector< XYPairInt16 > > Parse(int GameID);
	void Close(){
		file_ << std::endl << "]}";
		file_.close();
	};
	void Open(const std::string &file){
		file_.open(file, std::ios::app);
	};
	int GetLatestGame(){
		return latest_game_;
	};
	int Getlatestgame(){
		return latest_game_;
	};
	
private:
	std::ofstream file_;
	int latest_game_;
	int gameID_;
	int game_length_;
	

};
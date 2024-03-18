#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

#include "game_util.hpp"

class Logger
{
public:
	Logger(const int &GameID);
	~Logger();
	int Put(std::vector<std::pair<std::string, std::string> > input);
	std::vector<std::vector< XYPairInt16 > > Parse(int GameID);
	void Close();
	void Open(const std::string &file);
	uint32_t GetLatestGame();
private:
	std::ofstream file_;
	uint32_t latest_game_;
	int gameID_;
	int game_length_;
	int frame_ID_;
};
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "game_util.hpp"
#include <filesystem>

class Logger{

public:
	Logger(const int GameID);
	~Logger();
	int Put(std::vector<std::pair<uint16_t, uint16_t> > input);
	std::vector<std::vector< XYPairInt16 > > Parse(int GameID);
	void Close();
	void Open(const int game_id);
	bool IsOpen();
	static uint32_t GetLatestGame();


private:
	std::ofstream file_;
	int gameID_;
	int game_length_;
	int frame_ID_;


};

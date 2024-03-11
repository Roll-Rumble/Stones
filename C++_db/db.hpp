#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Logger{

public:
	Logger(const int &file){
		std::string filename = "Storage" + std::to_string(file) + ".json";
		file_.open(filename, std::ios::app);
	};
	~Logger(){
		file_ << std::endl << "]}";
		file_.close();
	};
	int Put(std::vector<std::pair<std::string, std::string> > input, int &FrameID);
	static std::vector<std::vector<std::pair< std::string, std::string> > > Parse(int FrameID, int GameID);
	void Close(){
		file_ << std::endl << "]}";
		file_.close();
	};
	void Open(const std::string &file){
		file_.open(file, std::ios::app);
	};

private:
	std::ofstream file_;

};
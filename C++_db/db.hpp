#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Logger{

public:
	Logger(const std::string &file){
		file_.open(file, std::ios::app);
	};
	~Logger(){
		file_.close();
	};
	int Put(std::vector<std::pair<std::string, std::string> > input, int &FrameID);
	static std::vector<std::pair< std::string, std::string> > Get(int FrameID);

private:
	std::ofstream file_;

};
#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <cstdint>

class Controller {
public:
	int16_t getXVal();
	int16_t getYVal();
	int16_t getZVal();


	void startNewThread();
	void operator()();

private:
	//void readJTAG();

	short x_val;
	short y_val;
	short z_val;
};

#endif
#ifndef READ_CONTROLLER_HPP
#define READ_CONTROLLER_HPP

#include <cstdint>
#include <jtag_atlantic.h>

#define READ_BUFFER_SIZE 6

struct XY_Pair {
	int16_t x;
	int16_t y;
};

class Controller {
public:
	Controller();
	~Controller();

	// Returns xy_accel_ value held by object
	XY_Pair get_xy_accel();
	// Reads controller and writes to xy_accel_ value held by object
	void read_xy_accel();

private:
	XY_Pair accel_;
	JTAGATLANTIC* handle_;
};

#endif

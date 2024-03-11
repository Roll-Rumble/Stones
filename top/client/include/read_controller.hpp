#ifndef READ_CONTROLLER_HPP
#define READ_CONTROLLER_HPP

#include "game_util.hpp"
#include <cstdint>
#include <jtag_atlantic.h>

#define CONTROLLER_BUF_SIZE 7

class Controller {
public:
	Controller();
	~Controller();

	// Returns xy_accel_ value held by object
	XYPairInt16 get_xy_accel() const;

	// Return true when buttons are pressed
	bool top_button_pressed();
	bool bottom_button_pressed();

	// Reads controller and writes to accel and button status
	void read_inputs();

private:
	XYPairInt16 accel_;
	bool top_button_;		// High when pressed
	bool bottom_button_;	// High when pressed
	JTAGATLANTIC* handle_;
};

#endif

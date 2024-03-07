#ifndef READ_CONTROLLER_HPP
#define READ_CONTROLLER_HPP

#include <jtag_atlantic.h>
#include <utility>

#define STR_BUFFER_SIZE 50
#define READ_BUFFER_SIZE 1

#define INPUT_THRESHOLD 20
#define OUTLIER_THRESHOLD 256

class Controller {
public:
	Controller();
	~Controller();

	short getXVal();
	short getYVal();
	short getZVal();

	std::pair<int16_t,int16_t> get_xy();

	static std::pair<float, float> normalise_xy(float x, float y);

private:
	//void readJTAG();

	short x_val;
	short y_val;
	short z_val;
	JTAGATLANTIC* handle;
};

#endif

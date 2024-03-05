#include "networking.hpp"
#include "Ball_test.hpp"
#include "Wall_test.hpp"
#include "Collision_detect.hpp"


int main()
{
	
	// int wall_num =1;
	Ball ball(0, 0, 0.1);
	// std::vector<Wall> walls;
	// Wall wall1(-1, -1, 1, 1);
	// walls.push_back(wall1);

	int input_x = 1; // get from accelerometer
	int input_y = 1;
	
	ball.Update(input_x, input_y);

	
	

	//int_fast8_t decision = Collision_detect(ball, wall1);
	//std::cout << decision << std::endl;
	return 0;
}
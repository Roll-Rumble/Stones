#include "Wall_test.hpp"

bool Wall::Hitbox_detect(Ball *ball) {
	if (ball->Get_location_x() >= location_x_ && ball->Get_location_x() <= (location_x_ +length))
	{
		if (ball->Get_location_y() >= location_y_ && ball->Get_location_y() <= (location_y_ + width))
		{
			return 1;
		}
	}	
	return 0;
}
#ifndef COLLISION_DETECT_HPP
#define COLLISION_DETECT_HPP

#include "Ball_test.hpp"
#include "Wall_test.hpp"

int_fast8_t Collision_detect(Ball ball, Wall wall){
	int_fast8_t decision = 0;
	for(int i = 0; i < 4; i++){ // runs through hitboxes

		if(ball.hitbox_[i].x >= wall.min_x  
		&& ball.hitbox_[i].x <= wall.max_x 
		&& ball.hitbox_[i].y >= wall.max_y 
		&& ball.hitbox_[i].y <= wall.min_y)
		{ 
			decision += i;
		}
		// 1, 2, 3, 4, 6, 10, are possible combinations of hitboxes in rectangles
		
	}
	if (decision > 4){
		return 100;
	}
	return decision;
}

#endif
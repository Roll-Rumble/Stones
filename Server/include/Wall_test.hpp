#include "Ball_test.hpp"

class Wall {
private:
    int length = 100;
    int width = 100;
	int location_x_ = 0; // location for a wall should be the top left corner
	int location_y_ = 0; // this makes hitbox calcs simpler

public:
	float max_x = location_x_ + length;
	float max_y = location_y_ + width;
	float min_x = location_x_;
	float min_y = location_y_;
    Wall(int length, int width) : length(length), width(width) {}

    // Add any additional member functions or variables here
	bool Hitbox_detect(Ball *ball) {} // it may make more sense for the ball to have this function
};


// 1 = wall 0 = floor ^ = entrance * = exit
/*
[ 1 1 1 1 1 1 1 1]      // the idea behind wall calling the collision detection is that the wall 
[ 1 0 0 0 0 0 0 1]		// will know which ball is nearby based on the ball coordinates. 
[ 1 0 0 1 1 0 0 1]		// for example if we round the balls location to the nearest intersection of 4
[ 1 0 ^ 1 1 0 0 1]		// squares we can then have the walls in those squares have their detection called
[ 1 1 1 1 1 0 0 1]		// on a success we know a collsiion and we can return the hitbox ID
[ 1 1 1 * 0 0 0 1]		// Now we know which wall was hit and in what direction so can call the collision function
[ 1 1 1 0 0 0 0 1]		// assuming an array it is easy to see which wall is in danger of being hit 
[ 1 1 1 1 1 1 1 1]		// using basic addition in the array from the nearest square to the ball



*/

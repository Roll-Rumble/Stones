#ifndef HITBOX_HPP
#define HITBOX_HPP

#include "Hitbox.hpp"

#include <iostream>

#define CIRCLE_SEGS 100

class Ball
{
private:
	/* data */
	int ball_ID_;
	float location_x_ = 0.0f;
	float location_y_ = 0.0f;
	float velocity_x_ = 0.0f;
	float velocity_y_ = 0.0f; // 0 index is x and 1 speed index is y speed
	
	
	bool mobile_ = 0;
	float radius_ = 5; 

	unsigned int buffer_;	// this is for the graphics
	
public:

	Hitbox hitbox_1;
	Hitbox hitbox_2;
	Hitbox hitbox_3;
	Hitbox hitbox_4;
	Hitbox hitbox_[4] = {hitbox_1, hitbox_2, hitbox_3, hitbox_4};

	Ball(float x, float y, float r) : location_x_(x), location_y_(y), radius_(r)
	{
	hitbox_1.x = location_x_ + radius_;
	hitbox_1.y = location_y_;
	hitbox_1.ID = 1;
	hitbox_2.x = location_x_ - radius_;
	hitbox_2.y = location_y_;
	hitbox_2.ID = 2;
	hitbox_3.x = location_x_;
	hitbox_3.y = location_y_ + radius_;
	hitbox_3.ID = 3;
	hitbox_4.x = location_x_;
	hitbox_4.y = location_y_ - radius_;
	hitbox_4.ID = 4;
	}
	~Ball();

	

 //			*				4
 //		*		*		3		1	// this is basic hitbox and id
 //			*				2
	void Collision(Hitbox hitbox); // idea here is that the id tells the function which direction the collision has occurred

	void Stop();

	void Hold();

	void Accelerate(int input_x, int input_y);

	void Update(int input_x, int input_y);

	float Get_location_x();

	float Get_location_y();
};


#endif
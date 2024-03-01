#include "Ball_test.hpp"

void Ball::Collision(int hitbox_ID) // idea here is that the id tells the function which direction the collision has occurred 
	{ 
		switch (hitbox_ID){
			case 1 : velocity_x_ = -velocity_x_;
			case 2 : velocity_y_ = -velocity_y_;
			case 3 : velocity_x_ = -velocity_x_;
			case 4 : velocity_y_ = -velocity_y_;
		} // this is obviously a very basic version
	}

		void Ball::Accelerate(int velocity_x_, int velocity_y_, int input_x, int input_y){
		int index; // placeholder
		int acceleration = 1; // placeholder
		if(input_x < 0){
			// negative acceleration
			velocity_x_ -= acceleration;
		}
		else if(input_x > 0){
			// positive acceleration
		}
		if (input_y < 0){
			// negative acceleration
		}
		else if(input_y > 0){
			// positive acceleration
		}
		else return; //acceleration is zero so no change needed
	}

	void Ball::Stop()
	{
		velocity_x_, velocity_y_ = 0;
	}

	void Ball::Hold()
	{
		velocity_x_, velocity_y_ = 0;
		mobile_ = 0;
	}

	int Ball::Get_location_x()
	{
		return location_x_;
	}

	int Ball::Get_location_y()
	{
		return location_y_;
	}

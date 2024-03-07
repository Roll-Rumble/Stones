#include "Ball.hpp"

void Ball::Collision(Hitbox hitbox) // idea here is that the id tells the function which direction the collision has occurred 
	{ 
		switch (hitbox.ID){
			case 1 : velocity_x_ = -velocity_x_;
			case 2 : velocity_y_ = -velocity_y_;
			case 3 : velocity_x_ = -velocity_x_;
			case 4 : velocity_y_ = -velocity_y_;
		} // this is obviously a very basic version
	}

void Ball::Accelerate(int input_x, int input_y)
	{
		float normal_x = input_x/256;
		float normal_y = input_y/256;
		float acceleration_x = 0;
		float accelearation_y = 0;
		float modifier;
		acceleration_x += normal_x*modifier;
		accelearation_y += normal_y*modifier; 
		if(input_x < 0){
			// negative acceleration
			velocity_x_ -= acceleration_x;
		}
		else if(input_x > 0){
			// positive acceleration
			velocity_x_ += acceleration_x;
		}
		if (input_y < 0){
			// negative acceleration
			velocity_y_ -= accelearation_y;
		}
		else if(input_y > 0){
			// positive acceleration
			velocity_y_ += accelearation_y;
		}
		else return; //acceleration is zero so no change needed
	}

void Ball::Update(int input_x, int input_y)
	{	
		if(mobile_ == 1){
			Accelerate(input_x, input_y);
			location_x_ += velocity_x_;
			location_y_ += velocity_y_;
		}
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

	float Ball::Get_location_x()
	{
		return location_x_;
	}

	float Ball::Get_location_y()
	{
		return location_y_;
	}


class Ball
{
private:
	/* data */
	int ball_ID_;
	int location_x_ = 0;
	int location_y_ = 0;
	int velocity_x_ = 0;
	int velocity_y_ = 0; // 0 index is x and 1 speed index is y speed
	bool mobile_ = 0;
public:
	Ball() 
	{

	}
	~Ball();

 //			*				4
 //		*		*		3		1	// this is basic hitbox and id
 //			*				2
	void Collision(int hitbox_ID) // idea here is that the id tells the function which direction the collision has occurred 
	{ 
		
	}

	void Stop()
	{

	}

	void Hold()
	{

	}

	void Accelerate(int velocity_x_, int velocity_y_, int input_x, int input_y)
	{
		
	}

	int Get_location_x()
	{

	}

	int Get_location_y()
	{

	}
};



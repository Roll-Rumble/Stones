#include "Ball_test.cpp"
#include <vector>


class Ball
{
private:
	/* data */
	int *ball_ID;
	int location[2];
	int velocity[2]; // 0 index is x and 1 speed index is y speed
	bool mobile;
public:
	Ball(/* args */){
		velocity[0,1] = 0;
		mobile = 0;
	}
	~Ball();

 //			*				1
 //		*		*		2		3	// this is basic hitbox and id
 //			*				4
	void Collision(int hitbox_ID) // idea here is that the id tells the function which direction the collision has occurred 
	{ 

	}

	void Stop()
	{

	}

	void Hold()
	{

	}
}



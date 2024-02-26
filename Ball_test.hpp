#include "Ball_test.cpp"
#include <vector>


class Ball
{
private:
	/* data */
	int velocity[2]; // 0 index is x and 1 speed index is y speed
public:
	Ball(/* args */){
		
	}
	~Ball();

//			*				1
//		*		*		2		3	
//			*				4
	void collision(int velocity[2], int hitbox_id) // idea here is that the id tells the function which direction the collision has occurred 
	{ 
		switch (hitbox_id){
			case 1 : velocity[1] = -velocity[1];
			case 2 : velocity[0] = -velocity[0];
			case 3 : velocity[0] = -velocity[0];
			case 4 : velocity[1] = -velocity[1];
		}
	}
};

Ball::Ball(/* args */)
{
}

Ball::~Ball()
{
}

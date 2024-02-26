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
		switch (hitbox_ID){
			case 1 : velocity[1] = -velocity[1];
			case 2 : velocity[0] = -velocity[0];
			case 3 : velocity[0] = -velocity[0];
			case 4 : velocity[1] = -velocity[1];
		} // this is obviously a very basic version
	}

	void Accelerate(std::vector<int> input){
		int index; // placeholder
		if(input.at(index) < 0){
			// negative acceleration
		}
		else if(input.at(index) > 0){
			// positive acceleration
		}
		else return; //acceleration is zero so no change needed
	}

	void Stop(){
		velocity[0,1] = 0;
	}

	void Hold(){
		velocity[0,1] = 0;
		mobile = 0;
		
	}
};

Ball::Ball(/* args */)
{
}

Ball::~Ball()
{
}

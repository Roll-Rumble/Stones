#include "Hitbox.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "controller_Con.cpp"

#include <jtag_atlantic.h>
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

	glGenBuffers(1, &buffer_);
	}
	~Ball();

	

 //			*				4
 //		*		*		3		1	// this is basic hitbox and id
 //			*				2
	void Collision(Hitbox hitbox) // idea here is that the id tells the function which direction the collision has occurred 
	{ 
		
	}

	void Stop()
	{

	}

	void Hold()
	{

	}

	void Accelerate(int input_x, int input_y)
	{
		
	}

	void Update(int input_x, int input_y)
	{	
		
	}

	int Get_location_x()
	{

	}

	int Get_location_y()
	{

	}

	void draw()
    {
        float positions[CIRCLE_SEGS * 2];

        float theta, x_off, y_off;

        for (int i = 0; i < CIRCLE_SEGS; i++)
        {
            theta = 2.0f * 3.1415926f * float(i) / float(CIRCLE_SEGS);//get the current angle

            x_off = radius_ * cosf(theta);//calculate the x component
            y_off = radius_ * sinf(theta);//calculate the y component

            positions[i * 2] = location_x_ + x_off;
            positions[i * 2 + 1] = location_y_ + y_off;
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffer_);
        glBufferData(GL_ARRAY_BUFFER, CIRCLE_SEGS * 2 * sizeof(float), positions, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);


        glDrawArrays(GL_LINE_LOOP, 0, CIRCLE_SEGS);
    }
};



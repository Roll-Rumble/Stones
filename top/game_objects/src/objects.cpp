#ifdef CLIENT_COMPILE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include "objects.hpp"

#ifdef CLIENT_COMPILE

Rectangle::Rectangle(float x, float y, float w, float h)
	: x_(x), y_(y), w_(w), h_(h)
{
	glGenBuffers(1, &buffer_);
}


void Rectangle::draw()
{
    float positions[8] = {
        x_, y_,
        x_, y_ + h_,
        x_ + w_, y_ + h_,
        x_ + w_, y_,
    };

    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);


    glDrawArrays(GL_QUADS, 0, 4);
}

void Rectangle::set_pos(float x, float y)
{
    x_ = x;
    y_ = y;
}

void Rectangle::set_dims(float w, float h)
{
    w_ = w;
    h_ = h;
}


Circle::Circle(float x, float y, float r)
    : x_(x), y_(y), r_(r)
{
    glGenBuffers(1, &buffer_);
}

void Circle::draw()
{
    float positions[CIRCLE_SEGS * 2];

    float theta, x_off, y_off;

    for (int i = 0; i < CIRCLE_SEGS; i++)
    {
        theta = 2.0f * 3.1415926f * float(i) / float(CIRCLE_SEGS);//get the current angle

        x_off = r_ * cosf(theta);//calculate the x component
        y_off = r_ * sinf(theta);//calculate the y component

        positions[i * 2] = x_ + x_off;
        positions[i * 2 + 1] = y_ + y_off;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glBufferData(GL_ARRAY_BUFFER, CIRCLE_SEGS * 2 * sizeof(float), positions, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);


    glDrawArrays(GL_LINE_LOOP, 0, CIRCLE_SEGS);
}

void Circle::set_pos(float x, float y)
{
    x_ = x;
    y_ = y;
}

void Circle::set_radius(float r)
{
    r_ = r;
}

void Circle::move(float x_in, float y_in)
{
    x_ += SPEED * x_in;
    if (x_ < -1.0)
    {
        x_ += 2;
    } else if (x_ > 1.0) {
        x_ -= 2;
    }

    y_ += SPEED * y_in;
    if (y_ < -1.0)
    {
        y_ += 2;
    }
    else if (y_ > 1.0) {
        y_ -= 2;
    }
}

#endif

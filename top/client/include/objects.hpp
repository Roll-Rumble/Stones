#ifndef OBJECTS_H
#define OBJECTS_H

#define SPEED 0.001
#define CIRCLE_SEGS 100


class Rectangle
{
private:
    unsigned int buffer_;
    float x_, y_, w_, h_;
public:

    Rectangle(float x, float y, float w, float h);

    ~Rectangle() {}

    void draw();

    void set_pos(float x, float y);


    void set_dims(float w, float h);
};

class Circle
{
private:
    unsigned int buffer_;
    float x_, y_, r_;
public:

    Circle(float x, float y, float r);

    ~Circle() {}

    void draw();

    void set_pos(float x, float y);

    void set_radius(float r);
    void move(float x_in, float y_in);
};

#endif
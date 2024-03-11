#ifndef OBJECTS_H
#define OBJECTS_H

#include <utility>

#define SPEED 0.001
#define CIRCLE_SEGS 100

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080


enum class TileType {W, F};



class Rectangle
{
public:

    Rectangle(int x, int y, int w, int h);

    ~Rectangle() {}

    void draw();

    void set_pos(int x, int y);


    void set_dims(int w, int h);
private:
    unsigned int buffer_;
    float x_, y_, w_, h_;
};

class Circle
{
public:

    Circle(int x, int y, int r);

    ~Circle() {}

    void draw();

    void set_pos(int x, int y);

    void set_radius(int r);
private:
    unsigned int buffer_;
    float x_, y_;
    int r_;
};

#endif
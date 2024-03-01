#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <jtag_atlantic.h>
#include "controller.hpp"



#define CIRCLE_SEGS 100

class Rectangle
{
private:
    unsigned int buffer_;
    float x_, y_, w_, h_;
public:

    Rectangle(float x, float y, float w, float h)
        : x_(x), y_(y), w_(w), h_(h)
    {
        glGenBuffers(1, &buffer_);
    }

    ~Rectangle() {}

    void draw()
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

    void set_pos(float x, float y)
    {
        x_ = x;
        y_ = y;
    }

    void set_dims(float w, float h)
    {
        w_ = w;
        h_ = h;
    }
};

class Circle
{
private:
    unsigned int buffer_;
    float x_, y_, r_;
public:

    Circle(float x, float y, float r)
        : x_(x), y_(y), r_(r)
    {
        glGenBuffers(1, &buffer_);
    }

    ~Circle() {}

    void draw()
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

    void set_pos(float x, float y)
    {
        x_ = x;
        y_ = y;
    }

    void set_radius(float r)
    {
        r_ = r;
    }
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cerr << "can't init GLEW" << std::endl;
    
    Rectangle rect1(-0.5f, -0.5f, 1, 1);
    Circle circ1(-0.5f, -0.5f, 0.5f);

    Controller niosII;

    niosII.startNewThread();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        rect1.draw();
        circ1.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
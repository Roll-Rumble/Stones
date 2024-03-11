#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "objects.hpp"
#include "shader.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cerr << "can't init GLEW" << std::endl;
    
    //Rectangle rect1(-0.5f, -0.5f, 1, 1);
    Circle circ1(100, 100, 50);
    Rectangle rect(1000, 1000, 100, 100);

    Shader shader("color_shader_vs.txt", "color_shader_fs.txt");
    shader.Use(1.0, 0.0, 0.0, 1.0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        
        //rect1.draw();
        circ1.draw();
        rect.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
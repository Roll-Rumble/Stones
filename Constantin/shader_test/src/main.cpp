#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "ball.hpp"
#include "map.hpp"
#include "shader.hpp"
#include "texture.hpp"

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
    Map map;
    Ball ball(map);

    Shader shader("color_shader_vs.txt", "color_shader_fs.txt");
    Texture floor_tex("floor_texture.png");
    Texture wall_tex("wall_texture.png");
    Texture hole_tex("hole_texture.png");
    Texture ball_tex("metal-texture.png");

    floor_tex.Bind(FLOOR_SLOT);
    wall_tex.Bind(WALL_SLOT);
    hole_tex.Bind(HOLE_SLOT);
    ball_tex.Bind(BALL_SLOT);
    floor_tex.Bind(ENTRANCE_SLOT);
    floor_tex.Bind(EXIT_SLOT);

    shader.Bind();
    /* Loop until the Bindr closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        
        //rect1.draw();
        map.draw(shader);
        ball.draw(shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
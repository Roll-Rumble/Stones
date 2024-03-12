#include "ball.hpp"
#include "client_tcp.hpp"
#include "client_udp.hpp"
#include "game_util.hpp"
#include "map.hpp"
#include "read_controller.hpp"
#include <chrono>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>


int main() {

    // Initialize controller object for reading input
    Controller nios2;

    std::thread controller_read_thread(&Controller::read_loop, &nios2);
    // // Initialize network communication sockets
    // TCPClient tcpClient;
    // UDPClient udpClient;

    // Initialize game objects

    // Create window for rendering
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        exit(1);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(1);
    }



    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        exit(1);

    Shader shader("color_shader_vs.txt", "color_shader_fs.txt");
    Map map;
    Ball ball(map);


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Read accelerometer data from Nios II
        XYPairInt16 xy_accel_data = nios2.get_xy_accel();
        std::cout << "x: " << xy_accel_data.x << ", y: " << xy_accel_data.y << "\n";

        // // std::cerr<< "Controller thread: sending data over UDP\n";
        // // Send data from controller over UDP
        // std::cerr << "Sending data to server: " << (int) xy_accel_data.x << ", "
        //           << (int) xy_accel_data.y << "\n";
        // udpClient.send_xy(xy_accel_data.x, xy_accel_data.y);
        // std::cerr << "Controller thread: data sent over UDP\n";

        // std::cerr << "Render thread: waiting to receive data from server\n";
        // // Normalization currently done on server side
        // std::pair<float,float> xy_pos = udpClient.receive_xy();
        // std::cerr << "Render thread: data received: " << xy_pos.first << ", "
        //           << xy_pos.second << "\n";

        // std::cout << "input x is " << xy_accel_data.x << ", ";
        // std::cout << "input y is " << xy_accel_data.y << "\n";

        // Compute ball physics
        ball.set_velocity(normalize_accel(xy_accel_data));
        // ball.update_velocity();
        ball.update_position();
        ball.resolve_wall_collisions(map);

        // Render frame
        map.draw(shader);
        ball.draw(shader);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    // This will stop the program from ever terminating right now
    controller_read_thread.join();
}

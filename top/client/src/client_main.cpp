#include <chrono>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ball.hpp"
#include "client_tcp.hpp"
#include "client_udp.hpp"
#include "game_util.hpp"
#include "map.hpp"
#include "read_controller.hpp"
#include "shader.hpp"


int main() {

    // Instantiate controller object for reading input
    Controller nios2;

    // Start separate read thread
    std::thread controller_read_thread(&Controller::read_loop, &nios2);

    // Initialize network communication sockets
    TCPClient tcpClient;
    UDPClient udpClient;

    // Create window for rendering
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) {
        exit(1);
    }

    int16_t i = 0;


    // while (true) {
    //     auto start = std::chrono::steady_clock::now(); // Start counting

    //     // glClear(GL_COLOR_BUFFER_BIT);

    //     // std::cout << "Client sending packet " << (int) i << "\n";
    //     // udpClient.send_xy(i, i);
    //     // i++;

    //     // std::pair<float,float> prev_xy_pos = xy_pos;
    //     // xy_pos = udpClient.receive_xy(xy_pos);
    //     // if (xy_pos.first != prev_xy_pos.first) {
    //     //     std::cout << "Client received packet " << (int) xy_pos.first << "\n";
    //     // }

    //     // Send data from controller over UDP
    //     udpClient.send_xy(xy_accel_data.x, xy_accel_data.y);

    //     // Receive position data over UDP
    //     xy_pos = udpClient.receive_xy(xy_pos);
    //     ball.set_position({xy_pos.first, xy_pos.second});

    //     // Render frame
    //     map.draw(shader);
    //     ball.draw(shader);

    //     // Swap front and back buffers
    //     glfwSwapBuffers(window);

    //     // Poll for and process events
    //     glfwPollEvents();
    //     auto end = std::chrono::steady_clock::now(); // End counting
    //     // Calculate the duration in milliseconds
    //     auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    //     // 10 FPS is ok

    //     while (elapsed.count() < 50000) { //wait until min amount has passed
    //         end = std::chrono::steady_clock::now();
    //         elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //     }
    //     // std::cout << "fps: " << 1000 / elapsed.count() << "\n";
    // }

    // // glfwTerminate();

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roll Rumble", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(1);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        exit(1);
    }

    // Load shaders
    Shader shader("color_shader_vs.txt", "color_shader_fs.txt");

    // Instantiate game objects
    Map map;
    Ball my_ball(map);
    Ball op_ball(map);

    std::pair<float,float> xy_pos = {0,0};

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        auto start = std::chrono::steady_clock::now(); // Start counting

        glClear(GL_COLOR_BUFFER_BIT);

        // Read accelerometer data from Nios II
        XYPairInt16 xy_tilt_input = nios2.get_xy_accel();

        // Calculate ball-wall physics
        my_ball.set_velocity(normalize_accel(xy_tilt_input));
        my_ball.update_position();
        my_ball.resolve_wall_collisions(map);

        // Send data from controller over UDP
        XYPairFloat ball_position = my_ball.get_position();
        udpClient.send_xy((int16_t) ball_position.x, (int16_t) ball_position.y);

        // // Receive position data over UDP
        // xy_pos = udpClient.receive_xy(xy_pos);
        // ball.set_position({xy_pos.first, xy_pos.second});

        std::pair<float,float> op_ball_pos =
        udpClient.receive_xy({op_ball.get_position().x, op_ball.get_position().y});
        op_ball.set_position({op_ball_pos.first, op_ball_pos.second});

        // Render frame
        map.draw(shader);
        my_ball.draw(shader);
        op_ball.draw(shader);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        auto end = std::chrono::steady_clock::now(); // End counting
        // Calculate the duration in milliseconds
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // 10 FPS is ok

        while (elapsed.count() < 50000) { //wait until min amount has passed
            end = std::chrono::steady_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        }
    }

    glfwTerminate();

    // This will stop the program from ever terminating right now
    controller_read_thread.join();
}

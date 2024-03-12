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
    Ball ball(map);

    std::pair<float,float> xy_pos = {0,0};

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        auto start = std::chrono::steady_clock::now(); // Start counting

        glClear(GL_COLOR_BUFFER_BIT);

        // Read accelerometer data from Nios II
        XYPairInt16 xy_accel_data = nios2.get_xy_accel();

        // // Send data from controller over UDP
        udpClient.send_xy(xy_accel_data.x, xy_accel_data.y);

        // Receive position data over UDP
        xy_pos = udpClient.receive_xy(xy_pos);
        ball.set_position({xy_pos.first, xy_pos.second});

        // Render frame
        map.draw(shader);
        ball.draw(shader);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
        auto end = std::chrono::steady_clock::now(); // End counting
        // Calculate the duration in milliseconds
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        while (elapsed.count() < 1000 / FPS) { //wait until min amount has passed
            end = std::chrono::steady_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        }
        std::cout << "fps: " << 1000 / elapsed.count() << "\n";
    }

    glfwTerminate();

    // This will stop the program from ever terminating right now
    controller_read_thread.join();
}

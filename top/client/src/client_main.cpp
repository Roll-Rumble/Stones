#include "client_threads.hpp"
#include "client_udp.hpp"
#include <functional>
#include <thread>

// Headers for everything
#include "client_tcp.hpp"
#include "client_udp.hpp"
#include "read_controller.hpp"
#include <cstdint>
#include <chrono>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "objects.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>


int main() {
    // // Instantiate UDP oject for server communication
    // UDP_Client udpClient;

    // // Initialize controller reader + server send thread
    // std::thread read_controller_thread(read_and_send_input, std::ref(udpClient));

    // // Initialize server read + render thread
    // std::thread read_server_thread(read_and_display_frame, std::ref(udpClient));

    // // Wait for threads to finish (they won't)
    // read_controller_thread.join();
    // read_server_thread.join();

    UDP_Client udpClient;

    // Initialize controller object for reading input
    Controller nios2;

    std::cerr << "Controller thread: creating TCP client\n";
    // Initialize TCP object and send init signal (string doesn't matter)
    TCP_Client tcpClient;
    std::cerr << "Controller thread: started TCP client\n";

    // Reads ball location from server and renders scene
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        exit(1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        exit(1);

    //Rectangle rect1(-0.5f, -0.5f, 1, 1);
    Circle circ1(-0.5f, -0.5f, 0.1f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // std::cerr << "Controller thread: reading from NIOS II\n";
        // Read accelerometer data from Nios II
        std::pair<int16_t,int16_t> xy_accel_data = nios2.get_xy();

        // std::cerr<< "Controller thread: sending data over UDP\n";
        // Send data from controller over UDP
        std::cerr << "Sending data to server: " << (int) xy_accel_data.first << ", "
                  << (int) xy_accel_data.second << "\n";
        udpClient.send_xy(xy_accel_data.first, xy_accel_data.second);
        std::cerr << "Controller thread: data sent over UDP\n";

        std::cerr << "Render thread: waiting to receive data from server\n";
        // Normalization currently done on server side
        std::pair<float,float> xy_pos = udpClient.receive_xy();
        std::cerr << "Render thread: data received: " << xy_pos.first << ", "
                  << xy_pos.second << "\n";

        circ1.move(xy_pos.first, xy_pos.second);

        //rect1.draw();
        circ1.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

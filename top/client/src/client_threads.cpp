#include "client_threads.hpp"

#include "client_tcp.hpp"
#include "client_udp.hpp"
#include "read_controller.hpp"
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "objects.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

void read_and_send_input(UDP_Client udpClient) {
    // Initialize controller object for reading input
    Controller nios2;

    // Initialize TCP object and send init signal (string doesn't matter)
    TCP_Client tcpClient;
    tcpClient.send_data("init");

    while (true) {
        // Read accelerometer data from Nios II
        std::pair<int16_t,int16_t> xy_accel_data = nios2.get_xy();

        // Send data from controller over UDP
        udpClient.send_xy(xy_accel_data.first, xy_accel_data.second);
    }
}

// Reads ball location from server and renders scene
void read_and_display_frame(UDP_Client udpClient) {
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

    if (glewInit() != GLEW_OK) exit(1);

    //Rectangle rect1(-0.5f, -0.5f, 1, 1);
    Circle circ1(-0.5f, -0.5f, 0.1f);

    Controller niosII;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        auto [x_in, y_in] = niosII.get_xy();
        auto [x_norm, y_norm] = Controller::normalise_xy(x_in, y_in);

        circ1.move(x_norm, y_norm);

        //rect1.draw();
        circ1.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

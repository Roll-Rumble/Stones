#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <jtag_atlantic.h>
#include "controller.hpp"
#include "objects.hpp"

#include "client_UDP.cpp"


void ReaderThread() {
    Controller niosII;

    WSAStartup();

    SOCKET clientSocket;
    SocketCreation(clientSocket);

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(12000); // Ensure the port is correct
    inet_pton(AF_INET, "35.178.225.231", &clientService.sin_addr); // Ensure the IP is correct

    while(true){
        auto [x_in, y_in] = niosII.get_xy();
        auto [x_norm, y_norm] = Controller::normalise_xy(x_in, y_in);

        send_data(clientSocket,clientService);
    }

}


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
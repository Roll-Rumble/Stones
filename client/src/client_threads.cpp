#include "client_threads.hpp"

#include "client_tcp.hpp"
#include "client_udp.hpp"
#include "read_controller.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

// Reads input from the controller and sends it to server
void read_and_send_input() {

    // Initialize controller object for reading input
    Controller Nios2;

    //
    WSAStartup();

    SOCKET clientSocket;
    SocketCreation(clientSocket);

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(12000); // Ensure the port is correct
    inet_pton(AF_INET, "35.178.225.231", &clientService.sin_addr); // Ensure the IP is correct


}

// Reads ball location from server and renders scene
void read_and_display_frame();

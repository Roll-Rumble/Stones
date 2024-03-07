#include "client_threads.hpp"

#include "client_tcp.hpp"
#include "client_udp.hpp"
#include "read_controller.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

// Reads input from the controller and sends it to server
void read_and_send_input() {
    // Initialize controller object for reading input
    Controller nios2;

    // Intiate use of winsock DLL
    WSAStartup();

    // Initialize TCP object
    // Send "init" (actual string doesn't matter) on TCP
    // If connection successful, continue (otherwise probably break/error)

    // Initialize UDP object

    while (true) {
        // Read controller
        // Send data from controller over UDP
    }
}

// Reads ball location from server and renders scene
void read_and_display_frame() {
    // Intiate use of winsock DLL
    WSAStartup();

    // Initialize UDP object

    while (true) {
        // Read data from UDP
        // Render frame using data
    }
}

#include "client_threads.hpp"
#include "client_udp.hpp"
#include <thread>

int main() {
    // Instantiate UDP oject for server communication
    UDP_Client udpClient;

    // Initialize controller reader + server send thread
    std::thread read_controller_thread(read_and_send_input, udpClient);

    // Initialize server read + render thread
    std::thread read_server_thread(read_and_display_frame, udpClient);

    // Wait for threads to finish (they won't)
    read_controller_thread.join();
    read_server_thread.join();

}

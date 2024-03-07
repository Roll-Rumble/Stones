#include "client_threads.hpp"
#include <thread>

int main() {

    // Initialize controller reader + server send thread
    std::thread read_controller_thread(read_and_send_input);

    // Initialize server read + render thread
    std::thread read_server_thread(read_and_display_frame);

    // Wait for threads to finish (they won't)
    read_controller_thread.join();
    read_server_thread.join();

}

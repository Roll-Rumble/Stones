#ifndef CLIENT_THREADS_HPP
#define CLIENT_THREADS_HPP

// Reads input from the controller and sends it to server
void read_and_send_input();

// Reads ball location from server and renders scene
void read_and_display_frame();

#endif

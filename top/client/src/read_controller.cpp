#include "read_controller.hpp"

#include <jtag_atlantic.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

Controller::Controller() : x_val (0), y_val (0) {
    handle = jtagatlantic_open(NULL, -1, -1, "jtag_read");
    int err;
    if (err = jtagatlantic_cable_warning(handle)) {
        std::cerr << "Error with connection: error code " << err << "\n";
        std::exit(1);
    }
}

Controller::~Controller() {
    jtagatlantic_close(handle);
}

short Controller::getXVal() {
	return x_val;
}

short Controller::getYVal() {
	return y_val;
}

std::pair<int16_t,int16_t> Controller::get_xy()
{
    unsigned int bytes_read;
    unsigned int bytes_available;

    static char str_buffer[STR_BUFFER_SIZE];
    static unsigned int str_buf_idx = 0;



    //std::cout << "called" << std::endl;
    // std::cout << "Bytes available to read is " << jtagatlantic_bytes_available(handle) << "\n";
    bytes_available = jtagatlantic_bytes_available(handle);
    //std::cout << "Bytes available: " << bytes_available << "\n";
    if (bytes_available) {
        str_buf_idx += jtagatlantic_read(handle, str_buffer + str_buf_idx, READ_BUFFER_SIZE);
        //read_buffer[bytes_read] = '/0';     // Create null-terminating string from input
        //std::cout << "Read " << bytes_read << "bytes\n";
        //std::cout << "Read data: " << *read_buffer << "\n";

        if (str_buf_idx > 1 && str_buffer[str // str_buffer[str_buf_idx - 1] == 0) {
            str_buffer[str_buf_idx - 1] = '\0';
            std::cout << "GOT RESULT: " << str_buffer << "\n";
            str_buf_idx = 0;
            sscanf(str_buffer, "X: %hd, Y: %hd, Z: %hd", &x_val, &y_val, &z_val);
            memset(str_buffer, '\0', STR_BUFFER_SIZE);
        }
    }

    return std::make_pair(static_cast<int16_t>(x_val), static_cast<int16_t>(y_val));
}

std::pair<float, float> Controller::normalise_xy(float x, float y)
{
    if (std::abs(x) < INPUT_THRESHOLD) {
        x = 0;
    }

    if (std::abs(y) < INPUT_THRESHOLD) {
        y = 0;
    }

    if (x > OUTLIER_THRESHOLD) {
        x = OUTLIER_THRESHOLD;
    }

    if (y > OUTLIER_THRESHOLD) {
        y = OUTLIER_THRESHOLD;
    }

    return std::make_pair(-x / OUTLIER_THRESHOLD, -y / OUTLIER_THRESHOLD);
}

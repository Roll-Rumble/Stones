#include "read_controller.hpp"

#include <jtag_atlantic.h>
#include <iostream>

Controller::Controller() : accel_{0}, top_button_(0), bottom_button_(0) {
    handle_ = jtagatlantic_open(NULL, -1, -1, "jtag_read");
    int err;
    if (err = jtagatlantic_cable_warning(handle_)) {
        std::cerr << "Error with connection: error code " << err << "\n";
        exit(1);
    }
}

Controller::~Controller() {
    jtagatlantic_close(handle_);
}

XYPairInt16 Controller::get_xy_accel() const {
    return accel_;
}

bool Controller::top_button_pressed() {
    if (top_button_) {
        top_button_ = 0;
        return 1;
    } else {
        return 0;
    }
}


bool Controller::bottom_button_pressed() {
    if (bottom_button_) {
        bottom_button_ = 0;
        return 1;
    } else {
        return 0;
    }}

void Controller::read_inputs() {
    unsigned int bytes_available;

    char buffer[CONTROLLER_BUF_SIZE];
    unsigned int buffer_idx = 0;

    // Continue attempting to read from JTAG until values found
    while (true) {
        bytes_available = jtagatlantic_bytes_available(handle_);
        //std::cout << "bytes available is " << bytes_available << "\n";
        if (bytes_available) {
            // jtagatlantic_read returns number of bytes read
            buffer_idx += jtagatlantic_read(handle_, buffer + buffer_idx, 1);

            if (buffer_idx > 1 && buffer[buffer_idx - 2] == -128
                               && buffer[buffer_idx - 1] == 0   ) {
                // Read x_val from buffer
                accel_.x = buffer[0];
                accel_.x <<= 8;
                accel_.x += buffer[1] & 0xff;

                // Read y_val from buffer
                accel_.y = buffer[2];
                accel_.y <<= 8;
                accel_.y += buffer[3] & 0xff;

                // Read button status from buffer
                // (only set low again once button value is read)
                top_button_ |= (buffer[4] & 0b01);
                bottom_button_ |= buffer[4] & 0b10;

                return;
            }
        }

        buffer_idx %= CONTROLLER_BUF_SIZE;
    }
}

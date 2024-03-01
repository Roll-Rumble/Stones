#include <jtag_atlantic.h>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>


#include "controller.hpp"


//#define STR_BUFFER_SIZE 50
//#define READ_BUFFER_SIZE 1

int16_t Controller::getXVal()
{
	return x_val;
}

int16_t Controller::getYVal()
{
	return y_val;
}

int16_t Controller::getZVal()
{
	return z_val;
}

void Controller::startNewThread()
{
    std::thread read_thread(*this);
}

void Controller::operator()()
{
    const char* progname = "jtag_read";
    JTAGATLANTIC* handle = jtagatlantic_open(NULL, -1, -1, progname);

    //char read_buffer[READ_BUFFER_SIZE + 1];
    //read_buffer[READ_BUFFER_SIZE] = '/0';
    //unsigned int bytes_read;

    char read_buffer[1];
    unsigned int bytes_read;
    unsigned int bytes_available;

    char str_buffer[50];
    unsigned int str_buf_idx = 0;

    if (jtagatlantic_cable_warning(handle)) {
        std::cout << "Error with connection: error code " << jtagatlantic_cable_warning(handle) << "\n";
        assert(0);
    }

    while (true) {
        // std::cout << "Bytes available to read is " << jtagatlantic_bytes_available(handle) << "\n";
        bytes_available = jtagatlantic_bytes_available(handle);
        //std::cout << "Bytes available: " << bytes_available << "\n";
        if (bytes_available) {
            bytes_read = jtagatlantic_read(handle, read_buffer, 1);
            //read_buffer[bytes_read] = '/0';     // Create null-terminating string from input
            //std::cout << "Read " << bytes_read << "bytes\n";
            //std::cout << "Read data: " << *read_buffer << "\n";
            str_buffer[str_buf_idx] = *read_buffer;
            str_buf_idx++;
            if (*read_buffer == 'e') {
                str_buffer[str_buf_idx - 1] = '\0';
                std::cout << "GOT RESULT: " << std::string(str_buffer) << "\n";
                str_buf_idx = 0;
                sscanf(str_buffer, "X: %i, Y: %i, Z: %i", &(this->x_val), &(this->y_val), &(this->z_val));
            }
        }
        else {
            //std::cout << "Wasted cycle!\n";
        }

    }

    std::cout << "Error: JTAG connection broken\n";
}
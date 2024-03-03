// JTAG_UART_test.cpp : Defines the entry point for the application.
//

#include "controller.hpp"

#include "jtag_atlantic.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

int main()
{
	Controller niosII;

	niosII.startNewThread();
}


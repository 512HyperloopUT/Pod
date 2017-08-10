//#include "comm/comm.h"
//#include "utils/utils.h"

//#include <string>
//#include <iostream>

#include "rpi-hw.hpp"
#include "rpi-hw/time.hpp"
#include "rpi-hw/gpio.hpp"

using namespace rpihw;

int main() {
	gpio &io = gpio::get();
	io.setup(11, OUTPUT);
	while (true) {
		io.write(11, HIGH);
	}
	return 0;
	/*
#ifndef __arm__
	printf("Warning: probably not running on Pi, not running\n");
	return 0;
#endif

	initComms();
	std::cout << "set up pod" << std::endl;

	int test[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	std::cout << "Enter a key to stop." << std::endl;

	setRead(1);

	while (!kbhit()) {
		//writeUpdate(test);
		//std::cout << "UDP WRITE: " << writeUpdate(test) << std::endl;
		float val = readUART();
		std::cout << "UART READ: " << val << std::endl;
	}

	return 0;
	*/
}

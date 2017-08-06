#include "comm/comm.h"
#include "gpio/GPIOPin.h"
#include "utils/utils.h"

#include <string>
#include <iostream>

int main() {
#ifndef __arm__
	printf("Warning: probably not running on Pi, disabling GPIO and UART\n");
#endif

	initComms();
	std::cout << "set up pod" << std::endl;

	int test[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	std::cout << "Enter a key to stop." << std::endl;

	while (!kbhit()) {
		writeUpdate(test);
		//std::cout << "UDP WRITE: " << writeUpdate(test) << std::endl;
		std::cout << "UART READ: " << readUARTRaw() << std::endl;
	}

	/*
	while (1) {
		std::cout << "Would you like to (r)ead a sensor or (w)rite to a sensor? You can also (q)uit." << std::endl;
		char selection, int id;
		std::cin >> selection;
		if (selection == 'q') {
			break;
		}
		std::cout << "Enter a sensor or actuator id for the " << selection=='r'?"read":"write" << " command." << std::endl;
		std::cin >> id;
		if (selection == 'w') {
			std::cout << "Enter either on (1) or off (0)." << std::endl;
			int state;
			std::cin >> state;
			std::cout << "Writing to actuator " << id << " the state of " << state << "." << std::endl;
			write(state, id);
		} else {
			std::cout << "Reading sensor " << id << "." << std::endl;
			setRead(id);
			std::cout << "Read value: " << readUART() << "." << std::endl;
		}
	}
	*/

	destroyComms();
	std::cout << "freed pod resources" << std::endl;

	return 0;
}

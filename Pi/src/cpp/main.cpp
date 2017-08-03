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
	int test[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	while (!kbhit()) {
		writeUpdate(test);
	}

	destroyComms();

	return 0;
}

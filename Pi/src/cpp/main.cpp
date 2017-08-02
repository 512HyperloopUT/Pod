#include <string>
#include <iostream>

#include "comm/comm.h"
#include "gpio/GPIOPin.h"

int main() {
#ifdef __arm__
	initComms();
#else
	printf("Warning: probably not running on Pi, disabling GPIO and UART\n");
#endif

	return 0;
}

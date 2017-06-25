//#include <stdio.h>

#include "comm/comm.h"
#include "gpio/GPIOPin.h"

int main() {
	int current = 0;

	GPIOPin readPin(1);
	GPIOPin writePin(2);

	while (true) {
		if (current != readPin.getValue()) {
			current = readPin.getValue();
			writePin.setValue(!writePin.getValue());
		}
	}
	return 0;
}

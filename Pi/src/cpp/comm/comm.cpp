#include "comm/comm.h"
#include "gpio/GPIOPin.h"

#include <vector>
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

struct pin_manager {
	//comm ports
	vector<GPIOPin> id_block;//for writing sensor ID
	GPIOPin r_pin;//for starting read command
	GPIOPin e_pin;//for ending reading command

	//anything else
	vector<GPIOPin> dwrite_block;//for writing to digital outputs
} pm;

class uart_manager {
} uart;

void initComms() {
	pm.id_block.push_back(GPIOPin(0));
	pm.id_block.push_back(GPIOPin(0));
	for_each(pm.id_block.begin(), pm.id_block.end(), [](GPIOPin& pin){
		pin.setDirection(OUTPUT);
	});

	pm.r_pin.setPort(0);
	pm.r_pin.setDirection(OUTPUT);

	pm.e_pin.setPort(0);
	pm.e_pin.setDirection(INPUT);
}

void setRead(int id) {
	int off = 0;
	for_each(pm.id_block.begin(), pm.id_block.end(), [id, off](GPIOPin& pin) {
		pin.setValue((id >> off) & 0x1);
	});
}

float read() {
}

void write(bool val, int id) {
	if (id < 0 || id >= pm.dwrite_block.size()) {
		return;
	}

	pm.dwrite_block[id].setValue(val);
}

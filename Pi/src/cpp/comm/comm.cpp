#include "comm/comm.h"
#include "comm/network.h"
#include "comm/uart.h"
#include "gpio/GPIOPin.h"

#include "boost/asio.hpp"

#include <vector>
#include <algorithm>
#include <functional>

/**********
internal code
**********/

//comm ports
std::vector<GPIOPin> id_block;//for writing sensor ID
GPIOPin r_pin(27, OUTPUT);//for starting read command
GPIOPin e_pin(28, INPUT);//for ending reading command

//digital write/read pins
std::vector<GPIOPin> dwrite_block;//for writing to digital outputs

//com api
int com_id;

//network
Network network;

/**********
public code
**********/

void initComms() {
	id_block.push_back(GPIOPin(29, OUTPUT));
	id_block.push_back(GPIOPin(31, OUTPUT));
	id_block.push_back(GPIOPin(33, OUTPUT));
	id_block.push_back(GPIOPin(35, OUTPUT));
	id_block.push_back(GPIOPin(37, OUTPUT));

	com_id = open_com("/dev/ttyACM0");
}

void setRead(int id) {
	r_pin.setValue(false);

	printf("resetting TM4C command\n");
	while (e_pin.getValue() != false) {}

	printf("setting sensor pins\n");
	int off = 0;
	std::for_each(id_block.begin(), id_block.end(), [id, off](GPIOPin& pin) {
		pin.setValue((id >> off) & 0x1);
	});

	r_pin.setValue(true);

	printf("writing for TM4C response\n");
	while (e_pin.getValue() != true) {}
}

float readUART() {
	printf("waiting for COMM_EOC to start buffer\n");
	while (read_uart(com_id, 1)[0] != COMM_EOC) {}

	printf("reading buffer\n");

	int val = 0;

	for (int off = 24; off >= 0; off -= 8) {
		char curr = read_uart(com_id, 1)[0];
		val = val | (curr << off);
	}

	read_uart(com_id, 1)[0];

	return val;
}

void write(bool val, int id) {
	if (id < 0 || id >= dwrite_block.size()) {
		return;
	}

	dwrite_block[id].setValue(val);
}

std::string writeUpdate(int* data) {
	std::string str = "";
	for (int i = 0; i < 10; i++) {
		str += std::to_string(data[i]) + " ";
	}

	network.send(str);

	return str;
}

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

//anything else
std::vector<GPIOPin> dwrite_block;//for writing to digital outputs

//com api
int com_id;

/**********
public code
**********/

void initComms() {
	id_block.push_back(GPIOPin(29, OUTPUT));
	id_block.push_back(GPIOPin(31, OUTPUT));
	id_block.push_back(GPIOPin(33, OUTPUT));
	id_block.push_back(GPIOPin(35, OUTPUT));
	id_block.push_back(GPIOPin(37, OUTPUT));

#ifdef __arm__
	com_id = open_com("/dev/ttyACM0");//TODO:fix
#endif

	initNetwork();
}

void setRead(int id) {
	int off = 0;
	std::for_each(id_block.begin(), id_block.end(), [id, off](GPIOPin& pin) {
		pin.setValue((id >> off) & 0x1);
	});
}

float readUART() {
#ifndef __arm__
	return 0;
#endif
	int x = 0;
	std::string output = read_uart(com_id, 100);
	//TODO: 4-byte float read protocol (check for terminating character)
}

std::string readUARTRaw() {
	return read_uart(com_id, 1);
}

void write(bool val, int id) {
	if (id < 0 || id >= dwrite_block.size()) {
		return;
	}

	dwrite_block[id].setValue(val);
}

int cnt = 0;

std::string writeUpdate(int* data) {
	std::string str = "";
	for (int i = 0; i < 10; i++) {
		str += std::to_string(data[i]) + " ";
	}

	send(str);

	return str + std::to_string(cnt++);
}

void destroyComms() {
	destroyNetwork();
}

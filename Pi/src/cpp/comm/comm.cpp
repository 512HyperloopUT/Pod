#include "comm/comm.h"
#include "comm/network.h"
#include "comm/uart.h"

#include "rpi-hw.hpp"
#include "rpi-hw/time.hpp"
#include "rpi-hw/gpio.hpp"

#include "boost/asio.hpp"

#include <vector>
#include <algorithm>
#include <functional>

using namespace rpihw;

/**********
internal code
**********/

/*
//comm ports
std::vector<GPIOPin> id_block;//for writing sensor ID
GPIOPin r_pin(0, OUTPUT);//for starting read command
GPIOPin e_pin(1, INPUT);//for ending reading command

//digital write/read pins
std::vector<GPIOPin> dwrite_block;//for writing to digital outputs
*/

gpio &io = gpio::get();

//com api
int com_id;

//network
Network network;

/**********
public code
**********/

void initComms() {
	io.setup(5, OUTPUT);
	io.setup(12, OUTPUT);
	io.setup(13, OUTPUT);
	io.setup(19, OUTPUT);
	io.setup(23, OUTPUT);

	io.setup(0, OUTPUT);
	io.setup(1, INPUT);

	com_id = open_com("/dev/ttyACM0");
}

void setRead(int id) {
	io.write(0, LOW);

	printf("resetting TM4C command\n");
	while (io.read(0) != LOW) {}

	printf("setting sensor pins\n");
	int off = 0;
	io.write(5, (id & 0x1) ? HIGH : LOW);
	io.write(5, (id & 0x10) ? HIGH : LOW);
	io.write(5, (id & 0x100) ? HIGH : LOW);
	io.write(5, (id & 0x1000) ? HIGH : LOW);
	io.write(5, (id & 0x10000) ? HIGH : LOW);

	io.write(0, HIGH);

	printf("writing for TM4C response\n");
	while (io.read(0) != HIGH) {}
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
	//output
}

std::string writeUpdate(int* data) {
	std::string str = "";
	for (int i = 0; i < 10; i++) {
		str += std::to_string(data[i]) + " ";
	}

	network.send(str);

	return str;
}

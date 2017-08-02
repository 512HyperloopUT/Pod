#include "comm/comm.h"
#include "comm/uart.h"
#include "gpio/GPIOPin.h"

#include "boost/asio.hpp"

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**********
internal code
**********/

//comm ports
vector<GPIOPin> id_block;//for writing sensor ID
GPIOPin r_pin;//for starting read command
GPIOPin e_pin;//for ending reading command

//anything else
vector<GPIOPin> dwrite_block;//for writing to digital outputs

//com api
int com_id;

/**********
public code
**********/

void initComms() {
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	for_each(id_block.begin(), id_block.end(), [](GPIOPin& pin){
		pin.setDirection(OUTPUT);
	});

	r_pin.setPort(0);
	r_pin.setDirection(OUTPUT);

	e_pin.setPort(0);
	e_pin.setDirection(INPUT);

	com_id = open_com("/dev/ttyUSB1");//TODO:fix
}

void setRead(int id) {
	int off = 0;
	for_each(id_block.begin(), id_block.end(), [id, off](GPIOPin& pin) {
		pin.setValue((id >> off) & 0x1);
	});
}

float readPort() {
	int x = 0;
	string output = read_port(com_id, 100);
	//TODO: 4-byte float read protocol (check for terminating character)
}

void write(bool val, int id) {
	if (id < 0 || id >= dwrite_block.size()) {
		return;
	}

	dwrite_block[id].setValue(val);
}

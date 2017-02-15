#include "gpio/GPIOPin.h"
#include "gpio/GPIOConstants.h"

#include <stdio.h>

#include <fstream>
#include <string>
#include <sstream>

GPIOPin::GPIOPin(int _port): port(_port) {}


bool GPIOPin::exportGPIO() {
	std::ofstream exportstream(EXPORT.c_str());
	if (exportstream < 0) {
		printf("Could not export GPIO pin %i\n", port);
		return true;
	}

	exportstream << port;
	exportstream.close();

	return false;
}

bool GPIOPin::unexportGPIO() {
	std::ofstream unexportstream(UNEXPORT.c_str());
	if (unexportstream < 0) {
		printf("Could not unexport GPIO pin %i\n", port);
		return true;
	}

	unexportstream << port;
	unexportstream.close();

	return false;
}

bool GPIOPin::setDirection(GPIODir dir) {
	std::ofstream dirstream(DIR(port).c_str());
	if (dirstream < 0) {
		printf("Could not set direction of GPIO pin %i\n", port);
		return true;
	}

	dirstream << (dir == INPUT ? "in" : "out");
	dirstream.close();

	return false;
}

bool GPIOPin::setValue(bool val) {
	std::ofstream valstream(VAL(port).c_str());
	if (valstream < 0) {
		printf("Could not set value of GPIO pin %i\n", port);
		return true;
	}

	valstream << val;//necessary?
	valstream.close();

	return false;
}

bool GPIOPin::getValue(bool& val) {
	std::ifstream valstream(VAL(port).c_str());
	if (valstream < 0) {
		printf("Could not read value of GPIO pin %i\n", port);
		return true;
	}

	int rval;
	valstream >> rval;
	val = rval;//necessary?

	valstream.close();

	return false;
}

int GPIOPin::getPort() {
	return port;
}

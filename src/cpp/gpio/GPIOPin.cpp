#include "gpio/GPIOPin.h"
#include "gpio/GPIOConstants.h"

#include <stdio.h>

#include <fstream>
#include <string>
#include <sstream>

GPIOPin::GPIOPin(int _port): port(_port) {
	exported = false;
}


bool GPIOPin::exportGPIO() {
	std::ofstream exportstream(EXPORT.c_str());
	if (exportstream < 0) {
		printf("Could not export GPIO pin %i\n", port);
		return true;
	}

	exportstream << port;
	exportstream.close();

	exported = true;

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

	exported = false;

	return false;
}

void GPIOPin::setDirection(GPIODir dir) {
	if (!exported) {
		return;
	}
	setDirection_s(dir);
}

void GPIOPin::setValue(bool val) {
	if (!exported) {
		return;
	}
	setValue_s(val);
}

bool GPIOPin::getValue() {
	if (!exported) {
		return 0;
	}
	bool ret;
	GPIOPin::getValue_s(ret);
	return ret;
}

bool GPIOPin::setDirection_s(GPIODir dir) {
	if (!exported) {
		return true;
	}
	std::ofstream dirstream(DIR(port).c_str());
	if (dirstream < 0) {
		printf("Could not set direction of GPIO pin %i\n", port);
		return true;
	}

	dirstream << (dir == INPUT ? "in" : "out");
	dirstream.close();

	return false;
}

bool GPIOPin::setValue_s(bool val) {
	if (!exported) {
		return true;
	}
	std::ofstream valstream(VAL(port).c_str());
	if (valstream < 0) {
		printf("Could not set value of GPIO pin %i\n", port);
		return true;
	}

	valstream << val;//necessary?
	valstream.close();

	return false;
}

bool GPIOPin::getValue_s(bool& val) {
	if (!exported) {
		return true;
	}
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

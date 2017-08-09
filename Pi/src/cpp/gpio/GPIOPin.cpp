#include "gpio/GPIOPin.h"
#include "gpio/GPIOConstants.h"

#include <stdio.h>

#include <string>
#include <fstream>
#include <sstream>

GPIOPin::GPIOPin() {
}

GPIOPin::GPIOPin(int port, GPIODir dir) {
	exportGPIO();
	setPort(port);
	setDirection(dir);
}

GPIOPin::~GPIOPin() {
	unexportGPIO();
}

void GPIOPin::setPort(int _port) {
	port = _port;
}

bool GPIOPin::exportGPIO() {
	std::ofstream exportstream(EXPORT);
#ifdef __arm__
	if (exportstream < 0) {
		printf("Could not export GPIO pin %i\n", port);
	}
#endif

	exportstream << port;
	exportstream.close();

	exported = true;

	return false;
}

bool GPIOPin::unexportGPIO() {
	std::ofstream unexportstream(UNEXPORT);
#ifdef __arm__
	if (unexportstream < 0) {
		printf("Could not unexport GPIO pin %i\n", port);
	}
#endif

	unexportstream << port;
	unexportstream.close();

	exported = false;

	return false;
}

void GPIOPin::setDirection(GPIODir dir) {
	setDirection_s(dir);
}

void GPIOPin::setValue(bool val) {
	setValue_s(val);
}

bool GPIOPin::getValue() {
	bool ret;
	getValue_s(ret);
	return ret;
}

bool GPIOPin::setDirection_s(GPIODir dir) {
	char str[50];
	DIR(str, port);
	std::ofstream dirstream(str);
#ifdef __arm__
	if (dirstream < 0) {
		printf("Could not set direction of GPIO pin %i\n", port);
	}
#endif

	dirstream << (dir == INPUT ? "in" : "out");
	dirstream.close();

	currentDir = dir;

	return false;
}

bool GPIOPin::setValue_s(bool val) {
	char str[50];
	VAL(str, port);
	std::ofstream valstream(str);
#ifdef __arm__
	if (valstream < 0) {
		printf("Could not set value of GPIO pin %i\n", port);
	}
#endif

	valstream << (val ? "1" : "0");//necessary?
	valstream.close();

	return false;
}

bool GPIOPin::getValue_s(bool& val) {
	char str[50];
	VAL(str, port);
	std::ifstream valstream(str);
#ifdef __arm__
	if (valstream < 0) {
		printf("Could not read value of GPIO pin %i\n", port);
		return true;
	}
#endif

	std::string strval;
	valstream >> strval;
	val = strval != "0";

	valstream.close();

	return false;
}

int GPIOPin::getPort() {
	return port;
}

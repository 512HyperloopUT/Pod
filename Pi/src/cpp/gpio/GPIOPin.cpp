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
	if (!exportstream.is_open()) {
		printf("Could not export GPIO pin %i\n", port);
	}

	exportstream << port;
	exportstream.close();

	exported = true;

	return false;
}

bool GPIOPin::unexportGPIO() {
	std::ofstream unexportstream(UNEXPORT);
	if (!unexportstream.is_open()) {
		printf("Could not unexport GPIO pin %i\n", port);
	}

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
	if (!exported) {
		return true;
	}
	char str[50];
	DIR(str, port);
	std::ofstream dirstream(str, std::ios::out | std::ios::trunc);
	if (!dirstream.is_open()) {
		printf("Could not set direction of GPIO pin %i\n", port);
	}

	dirstream << (dir == INPUT ? "in" : "out");
	dirstream.close();

	currentDir = dir;

	return false;
}

bool GPIOPin::setValue_s(bool val) {
	if (!exported) {
		return true;
	}
	char str[50];
	VAL(str, port);
	std::ofstream valstream(str, std::ios::out | std::ios::trunc);
	if (!valstream.is_open()) {
		printf("Could not set value of GPIO pin %i\n", port);
	}

	valstream << val;//necessary?
	valstream.close();

	return false;
}

bool GPIOPin::getValue_s(bool& val) {
	if (!exported) {
		return true;
	}
	char str[50];
	VAL(str, port);
	std::ifstream valstream(str, std::ios::in);
	if (!valstream.is_open()) {
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

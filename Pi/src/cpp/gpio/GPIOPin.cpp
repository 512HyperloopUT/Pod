#include "gpio/GPIOPin.h"
#include "gpio/GPIOConstants.h"

#include <stdio.h>

#include <string>
#include <fstream>
#include <sstream>

GPIOPin::GPIOPin() {
}

GPIOPin::GPIOPin(int _port) {
	setPort(_port);
}

void GPIOPin::setPort(int _port) {
	port = _port;
}

bool GPIOPin::exportGPIO() {
	ofstream exportstream(EXPORT.c_str());
	if (!exportstream.is_open()) {
		printf("Could not export GPIO pin %i\n", port);
		return true;
	}

	exportstream << port;
	exportstream.close();

	exported = true;

	return false;
}

bool GPIOPin::unexportGPIO() {
	ofstream unexportstream(UNEXPORT.c_str());
	if (!unexportstream.is_open()) {
		printf("Could not unexport GPIO pin %i\n", port);
		return true;
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
	ofstream dirstream(DIR(port).c_str());
	if (!dirstream.is_open()) {
		printf("Could not set direction of GPIO pin %i\n", port);
		return true;
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
	ofstream valstream(VAL(port).c_str());
	if (!valstream.is_open()) {
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
	ifstream valstream(VAL(port).c_str());
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

#include "sensor/sensor_types.h"
#include <stdio.h>

DigitalReader::DigitalReader(int port) {
	pin = new GPIOPin(port);
	pin->exportGPIO();
}

DigitalReader::~DigitalReader() {
	pin->unexportGPIO();
	delete pin;
}

double DigitalReader::getValue() {
	return pin->getValue();
}

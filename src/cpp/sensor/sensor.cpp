#include "sensor.h"

Sensor::Sensor() {
	value = 100;
}

Sensor::~Sensor() {
}

int Sensor::getValue() {
	return value;
}

#pragma once

#include "sensor/sensor.h"
#include "gpio/GPIOPin.h"

class DigitalReader : public Sensor {
private:
	GPIOPin* pin;
public:
	DigitalReader(int port);
	~DigitalReader();
	double getValue();
};

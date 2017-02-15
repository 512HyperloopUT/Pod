#pragma once

#include "filter/filter.h"

class Sensor {
private:
	int value;
public:
	Sensor();
	~Sensor();
	virtual int getValue();
};

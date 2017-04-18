#pragma once

#include "filter/filter.h"

class Sensor {
public:
	virtual double getValue() = 0;
};

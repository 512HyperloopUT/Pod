#include "pod/pod.h"

#include <stdio.h>

Pod* Pod::instance = 0;

Pod::Pod() {
}

Pod::~Pod() {
	sensors.clear();
}

void Pod::addSensor(Sensor sensor) {
	sensors.push_back(sensor);
}

std::vector<Sensor> Pod::getSensors() {
	return sensors;
}

Pod* Pod::getInstance() {
	if (!instance) {
		instance = new Pod;
	}
	return instance;
}


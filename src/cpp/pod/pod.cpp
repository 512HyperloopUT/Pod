#include "pod/pod.h"

Pod Pod::instance = *(new Pod);

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
	return &Pod::instance;
}


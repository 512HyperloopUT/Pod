#include "pod/sensor.h"
#include "comm/comm.h"


/*** generic sensor ***/

Sensor::Sensor(const int _id): dat_id(_id) {

}

Sensor::~Sensor() {

}

SensorDatapack Sensor::readSensor() {
	return this->dataProcess(read(this->dat_id));
}

SensorDatapack Sensor::dataProcess(float dat) {
	SensorDatapack sdp;
	sdp.f = dat;
	return sdp;
}

/*** specific sensors ***/
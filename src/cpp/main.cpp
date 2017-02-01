#include <stdio.h>

#include "sensor/sensor.h"
#include "pod/pod.h"

int main() {
	Sensor s1, s2, s3;
	Pod* pod = Pod::getInstance();
	pod->addSensor(s1);
	pod->addSensor(s2);
	pod->addSensor(s3);

	std::vector<Sensor> sensors = pod->getSensors();

	for (std::vector<Sensor>::iterator it = sensors.begin(); it != sensors.end(); it++) {
		printf("%d\n", it->getValue());
	}

	return 0;
}

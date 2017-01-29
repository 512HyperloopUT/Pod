#include <stdio.h>

#include "sensor/sensor.h"
#include "pod/pod.h"

int main() {

	Sensor* s1 = new Sensor;
	Sensor* s2 = new Sensor;
	Pod* pod = Pod::getInstance();
	pod->addSensor(s1);
	pod->addSensor(s2);

	std::vector<Sensor*> sensors = *(pod->getSensors());

	for (std::vector<Sensor*>::iterator it = sensors.begin(); it != sensors.end(); it++) {
		printf("%d\n", (*it)->getValue());
	}

	return 0;
}

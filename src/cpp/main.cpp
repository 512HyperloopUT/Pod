#include <stdio.h>

#include "sensor/sensor.h"
#include "pod/pod.h"
#include "filter/filter.h"
#include "gpio/GPIOPin.h"

int main() {
#ifndef __arm__
	printf("not running on Raspberry PI\n");
	return 0;
#endif

	GPIOPin pin(4);
/*
	Sensor s1;
	Pod* pod = Pod::getInstance();
	pod->addSensor(s1);

	std::vector<Sensor> sensors = pod->getSensors();

	for (std::vector<Sensor>::iterator it = sensors.begin(); it != sensors.end(); it++) {
		printf("%d\n", it->getValue());
	}
*/

/*
	AbsFilter afilter;
	std::vector<double> sample_inputs;
	sample_inputs.push_back(-11.0);
	printf("%f\n", afilter.process(sample_inputs).at(0));
*/
	//delete pod;

	return 0;
}

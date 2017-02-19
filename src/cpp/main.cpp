#include <stdio.h>

#include "pod/pod.h"
#include "sensor/sensor_types.h"

int main() {
#ifndef __arm__
	printf("probably not running on Raspberry PI\n");
	return 0;
#endif

	DigitalReader dr(4);

	Pod* pod = Pod::getInstance();
	pod->addSensor(&dr);

	delete pod;

	return 0;
}

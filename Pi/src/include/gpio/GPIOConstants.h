#pragma once

#include <string>

const char* EXPORT = "/sys/class/gpio/export";

const char* UNEXPORT = "/sys/class/gpio/unexport";

void GPIO_PIN(char* str, int port) {
	sprintf(str, "/sys/class/gpio/gpio%i", port);
}

void DIR(char* str, int port) {
	sprintf(str, "/sys/class/gpio/gpio%i/direction", port);
}

void VAL(char* str, int port) {
	sprintf(str, "/sys/class/gpio/gpio%i/value", port);
}

#pragma once

#include <string>

const char* EXPORT = "/sys/class/gpio/export";

const char* UNEXPORT = "/sys/class/gpio/unexport";

const std::string GPIO_PIN(int port) {
	return "/sys/class/gpio/gpio" + std::to_string(port);
}

const std::string DIR(int port) {
	return GPIO_PIN(port) + "/direction";
}

const std::string VAL(int port) {
	return GPIO_PIN(port) + "/value";
}

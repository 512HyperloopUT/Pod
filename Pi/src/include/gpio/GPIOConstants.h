#pragma once

#include <string>

const std::string GPIO_BASE = "/sys/class/gpio";
const std::string EXPORT = GPIO_BASE + "/export";
const std::string UNEXPORT = GPIO_BASE + "/unexport";
const std::string GPIO_PIN(int port) {
	return GPIO_BASE + "/gpio" + std::to_string(port);
}
const std::string DIR(int port) {
	return GPIO_PIN(port) + "/direction";
}
const std::string VAL(int port) {
	return GPIO_PIN(port) + "/value";
}

#pragma once

#include <string>

using namespace std;

const string GPIO_BASE = "/sys/class/gpio";

const string EXPORT = GPIO_BASE + "/export";

const string UNEXPORT = GPIO_BASE + "/unexport";

const string GPIO_PIN(int port) {
	return GPIO_BASE + "/gpio" + to_string(port);
}

const string DIR(int port) {
	return GPIO_PIN(port) + "/direction";
}

const string VAL(int port) {
	return GPIO_PIN(port) + "/value";
}

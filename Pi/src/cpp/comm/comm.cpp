#include "comm/comm.h"
#include "gpio/GPIOPin.h"

#include <vector>
using namespace std;

using half_float::half;

struct PinOut {
	vector<GPIOPin> right;
	vector<GPIOPin> rightmid;
	vector<GPIOPin> leftmid;
	vector<GPIOPin> left;

	PinOut() {
	}
} pinout;

float read(int id) {
	half a(3.98438);
	cout << a << endl;
	return a;
}

void write(float val, int id) {
}

float b12tof(uint16_t x) {
	return (float)x/(1<<11) - 1.0;
}


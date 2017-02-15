#pragma once

class GPIOPin {
private:
	int port;
public:
	GPIOPin(int port);
	bool exportGPIO();
	bool unexportGPIO();
	enum GPIODir{INPUT, OUTPUT};
	bool setDirection(GPIODir dir);
	bool setValue(bool val);
	bool getValue(bool& val);
	int getPort();
};

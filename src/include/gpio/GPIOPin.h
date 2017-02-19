#pragma once

class GPIOPin {
private:
	int port;
	bool exported;
public:
	GPIOPin(int port);
	bool exportGPIO();
	bool unexportGPIO();
	enum GPIODir{INPUT, OUTPUT};
	//just do the thing
	void setDirection(GPIODir dir);
	void setValue(bool val);
	bool getValue();
	//do the thing and return status
	bool setDirection_s(GPIODir dir);
	bool setValue_s(bool val);
	bool getValue_s(bool& val);
	int getPort();
};

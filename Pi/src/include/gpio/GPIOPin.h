#pragma once

enum GPIODir {
	INPUT,
	OUTPUT
};

class GPIOPin {
private:
	int port;
	bool exported;
	GPIODir currentDir;
public:
	GPIOPin();
	GPIOPin(int _port);
	//inits
	void setPort(int _port);
	bool exportGPIO();
	bool unexportGPIO();
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

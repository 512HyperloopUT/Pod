#pragma once

#ifndef PERIPH_H
#define PERIPH_H

#include <string>
#include <stdint>

/***************Pod interface*******************/

/*

<code>sensor</code> is used read an input from a local hardware device.

<code>actuator</code> is used write an output from a local hardware device. It is an abstract class.

Both of these classes use a static factory method.

<code>client</code> refers to and abstract connection to the pod hardware, combining GPIO, UART and the communication with the TM4C (which uses GPIO).

*/

class sensor {
public: //Should be private...
	sensor();
	virtual ~sensor();
public:
	std::string name;
public:
	static sensor* make_sensor(std::string sensor); //Factory method
	virtual void update_sensor(client *clt) = 0; //Read method
};

class actuator {
public: //Should be private...
	actuator();
	virtual ~actuator();
public:
	std::string name;
public:
	static actuator* make_actuator(std::string actuator); //Factory method
	virtual void update_actuator(client *clt, std::string input) = 0; //Write method
};

class client {
public:
	client();
	virtual ~client();
public:
	virtual void write(std::uint64_t id, std::uint64_t value) = 0;
	virtual std::uint32_t read(std::uint64_t id) = 0;
};

#endif
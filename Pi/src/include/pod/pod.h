#pragma once

class Pod {
public:
	Pod();
	~Pod();

	void init();//initalize sensors & pod
	void update();//update sensors (IO, algorithms, etc)
	void shutdown();//shutdown (delete pointers, sensors, etc)

	void addSensor();//add sensor
};

#include <vector>

#include "sensor/sensor.h"

class Pod {
private:
	//pod privates
	std::vector<Sensor> sensors;
	Pod();
	//singleton privates
	static Pod* instance;
public:
	~Pod();
	void addSensor(Sensor sensor);
	std::vector<Sensor> getSensors();
	//singleton access
	static Pod* getInstance();
};

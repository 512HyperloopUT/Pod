#pragma once


/* generic data package, sent from tm4c */
union SensorDatapack {
	char c;
	int i;
	long long ll;
	float f;
};

class Sensor {
public:
	/* constructor/deconstructor */
	Sensor(const int id);
	~Sensor();

	/* read processed data */
	SensorDatapack readSensor();

private:
	int dat_id;

	/* transform data, depend on sensor type */
	virtual SensorDatapack dataProcess(float dat);
};
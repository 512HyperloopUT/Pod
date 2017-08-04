#include <iostream>
#include <string>
#include <vector>
#include <stdint>
#include <chrono>

#include "periph.h"
#include "spacexnet.h"

//pod centralization class
class pod {
public:
	std::vector<sensor*> sensors;
	std::vector<actuator*> motors;
	client* cl;
	master* ms;
	std::vector<timed_action*> actions;
	std::string state; //TODO decide what states to use
public:
	pod();
	~pod();
public:
	void update_sensors();
	void update_actuators();
private:
	void log();
};

void pre_init(void){
	//TODO
}
pod init_pod(pod &p){
	//TODO add sensors
	//TODO add actuators
	//TODO add actions
}
client init_client(){
	//TODO client initialization
}
master init_master(){
	//TODO GUI/UDP initialization
}

pod p;
int braking_allowed = 0;

void init() {
	p = pod();
	p.cl = init_client();
	p.ms = init_master();
	init_pod(p);
}
void loop() {
	p.update_sensors();
	//TODO insert processing here
}

int stop_check(void) {
	master_packet* p.master.receive(); //TODO clear out the queue for the socket
	//TODO check for emergency stop
	//TODO check for other stops
	return 0;
}

int shutdown_check(void) {
	//TODO check to see if should shutdown
	return 0;
}

void graceful_shutdown(void) {
	//TODO, probably uninitialize
}

//Basic loop
int main() {
	init();
	
	std::uint8_t stop;
	std::uint8_t shutdown = 0;
	
	while (!shutdown) {
		stop = 0;
		while (!stop) {
			loop();
			stop = stop_check();
		}
		shutdown = shutdown_check();
	}
	
	graceful_shutdown();
	return 0;
}

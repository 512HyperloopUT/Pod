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
	//TODO any other inits
}

std::uint8_t pre_start() {
	//TODO recover from stop/begin moving
	return 0;
}

std::uint8_t loop() {
	p.update_sensors();
	//TODO insert processing here
	//TODO execute timed actions
	return stop_check();
}

std::uint8_t stop_check(void) {
	master_packet* packet = p.master.receive(); //TODO clear out the queue for the socket
	//TODO check for emergency stop
	//TODO check for other stops
	return 0;
}

void post_stop() {
	//TODO cooldown after braking
	return shutdown_check();
}

std::uint8_t shutdown_check(void) {
	//TODO check to see if should shutdown
	return 0;
}

std::uint8_t graceful_shutdown(void) {
	//TODO, probably uninitialize
	return 0;
}

//Basic loop
int main() {
	init();
	
	std::uint8_t stop;
	std::uint8_t shutdown = 0;
	
	while (!shutdown) {
		stop = pre_start();
		while (!stop) {
			stop = loop();
		}
		shutdown = post_stop();
	}
	
	return graceful_shutdown();;
}

#include <iostream>
#include <string>
#include <vector>
#include <stdint>
#include <chrono>

//Pod interface
class sensor {
public:
	sensor();
	virtual ~sensor();
public:
	static sensor* make_sensor(std::string sensor);
	virtual void update_sensor(client *clt) = 0;
};
class client {
public:
	client();
	virtual ~client();
public:
	virtual void write(std::uint64_t id, std::uint64_t value) = 0;
	virtual uint32_t read(std::uint64_t id) = 0;
};

//UDP/GUI classes
class master_packet {
public:
	master_packet();
	virtual ~master_packet();
};
class master {
public:
	virtual void send(master_packet& packet) = 0;
	virtual master_packet receive(void) = 0;
};

//Regular actions
class timed_action {
public:
	timed_action(uint64_t ns, int call_on_create) {
		ns_per_call = ns;
		last = std::chrono::high_resolution_clock::now();
		if (call_on_create) {
			call();
		}
	}
	virtual ~timed_action();
public:
	uint64_t ns_per_call;
private:
	time_point<high_resolution_clock> last;
public:
	int check_act(void) {
		int temp = last;
		return (last = std::chrono::high_resolution_clock::now() - temp) > ns_per_call;
	}
	virtual void act(pod &p) = 0;
};

//pod centralization class
class pod {
public:
	std::vector<sensor*> data;
	client* cl;
	master* ms;
	std::vector<timed_action*> actions;
	std::string state; //TODO decide what states to use
public:
	pod();
	~pod();
public:
	void update_sensors();
private:
	void log();
};

void pre_init(void){
	//TODO
}
pod init_pod(pod &p){
	//TODO add necessary sensors
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
	//TODO add sensors
	//TODO add actions
}
void loop() {
	p.update_sensors();
	//TODO insert processing here
}

int emergency_check_stop(void) {
	master_packet* p.master.receive(); //TODO clear out the queue for the socket
	//TODO check for emergency stop
	//TODO check for other stops
	return 0;
}

int shutdown_check(void) {
	//TODO check to see if should shutdown
	return 0;
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
	
	return 0;
}

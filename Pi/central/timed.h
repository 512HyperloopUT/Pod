#pragma once

#ifndef SPACEXNET_H
#define SPACEXNET_H

/*****************Scheduled actions******************/

/*

<code>timed_action</code> is a timed action that occurs every given number of nanoseconds. It is an abstract class.

*/
class timed_action {
public:
	timed_action(uint64_t ns) {
		ns_per_call = ns;
		last = std::chrono::high_resolution_clock::now();
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

#endif
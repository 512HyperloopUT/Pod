#pragma once

#ifndef SPACEXNET_H
#define SPACEXNET_H

#include <stdint>

/*****************Scheduled actions******************/

/*

<code>timed_action</code> is a timed action that occurs every given number of nanoseconds. It is an abstract class.

*/
class timed_action {
public:
	timed_action(std::uint64_t ns, std::int64_t times) {
		ns_per_call = ns;
		reps = 0;
		target_reps = times;
		last = std::chrono::high_resolution_clock::now();
	}
	virtual ~timed_action();
public:
	std::uint64_t ns_per_call;
private:
	std::int64_t target_reps;
	std::int64_t reps;
	std::time_point<std::chrono::high_resolution_clock> last;
public:
	std::uint8_t check_act_no_limit(void) {
		std::uint8_t temp = last;
		return (last = std::chrono::high_resolution_clock::now() - temp) > ns_per_call;
	}
	std::uint8_t check_act(void) {
		//TODO check for max possible value of int64_t
		return ((target_reps == -1)?(1):(reps >= target_reps)) && check_act_no_limit();
	}
	void reset(void) {
		reps = 0;
		last = std::chrono::high_resolution_clock::now();
	}
	std::uint8_t do_action(pod &p, std::uint8_t ignore_check) {
		if (ignore_check || check_act()) {
			act(p);
			reps++;
			return 1;
		}
		return 0;
	}
private:
	virtual void act(pod &p) = 0;
};

#endif
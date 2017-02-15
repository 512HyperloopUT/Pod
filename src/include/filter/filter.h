#pragma once

#include <vector>

class Filter {
public:
	virtual std::vector<double> process(std::vector<double> inputs) = 0;
};

class AbsFilter : public Filter {
public:
	std::vector<double> process(std::vector<double> inputs);
};

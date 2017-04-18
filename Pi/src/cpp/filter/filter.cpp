#include "filter/filter.h"

#include <cmath>
#include <algorithm>

std::vector<double> AbsFilter::process(std::vector<double> inputs) {
	std::for_each(inputs.begin(), inputs.end(), [](double& a) {a = fabs(a);});
	return inputs;
}

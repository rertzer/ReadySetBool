#include "powerset.hpp"
#include "printer.hpp"

uint64_t power(uint64_t x, uint64_t p) {
	uint64_t result;

	if (p == 0) {
		result = 1;
	} else if (p == 1) {
		result = x;
	} else {
		uint32_t tmp = power(x, p / 2);
		if ((p % 2) == 0) {
			result = tmp * tmp;
		} else {
			result = x * tmp * tmp;
		}
	}

	return (result);
}

vector<vector<int32_t>> powerset(vector<int32_t> set) {
	vector<vector<int32_t>> power_set;
	uint64_t				power_size;

	if (set.size() > 63) {
		throw(SizeException());
	}
	power_size = power(2, set.size());

	for (uint64_t i = 0; i < power_size; ++i) {
		vector<int32_t> subset;
		for (uint64_t j = 0; j < set.size(); ++j) {
			uint64_t mask = 1 << j;
			if ((i & mask) != 0) {
				subset.push_back(set[j]);
			}
		}
		power_set.push_back(subset);
	}

	return (power_set);
}

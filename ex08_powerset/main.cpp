#include "powerset.hpp"
#include "printer.hpp"

int main() {
	vector<vector<int32_t>> set_set = {{1},
									   {1, 2},
									   {1, 2, 3, 4},
									   {1, 2, 3, 4, 5, 6},
									   {1, 2, 3, 4, 5, 6, 7, 8},
									   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
									   /*{1,	2,	3,	4,	5,	6,	7,	8,	9,	10, 11, 12, 13,
										14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}*/};
	vector<vector<int32_t>> power_set;

	for (auto set : set_set) {
		try {
			power_set = powerset(set);
			cout << "set: " << set << "\n";
			cout << "powerset: " << power_set << "\n\n";
		} catch (SizeException& e) {
			cout << e.what() << "\n\n";
		}
	}
}

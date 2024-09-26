#include "eval_set.hpp"

uint32_t power(uint32_t x, uint32_t p) {
	uint32_t result;

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

vector<int32_t> eval_set(string const& formula, vector<vector<int32_t>> sets) {
	vector<int32_t> soluce;
	string			alphabet = get_alphabet(formula);
	set<int32_t>	union_set = get_union_set(sets);

	for (auto number : union_set) {
		uint32_t subset = get_subset(sets, number);
		string	 valued_formula = value_formula(formula, alphabet, subset);
		bool	 ok = eval_formula(valued_formula);
		if (ok == true) {
			soluce.push_back(number);
		}
	}
	return (soluce);
}

string get_alphabet(string const& formula) {
	string alphabet;
	bool   alpha_table[26] = {false};

	for (auto symbol : formula) {
		if (symbol >= 'A' && symbol <= 'Z') {
			alpha_table[symbol - 'A'] = true;
		}
	}

	for (int i = 0; i < 26; ++i) {
		if (alpha_table[i] == true) {
			alphabet += 'A' + i;
		}
	}
	return (alphabet);
}

set<int32_t> get_union_set(vector<vector<int32_t>> const& sets) {
	set<int32_t> union_set;
	for (auto set : sets) {
		for (auto item : set) {
			union_set.insert(item);
		}
	}
	return (union_set);
}

uint32_t get_subset(vector<vector<int32_t>> sets, int32_t number) {
	uint32_t subset = 0;
	for (size_t i = 0; i < sets.size(); ++i) {
		auto it = find(sets[i].begin(), sets[i].end(), number);
		if (it != sets[i].end()) {
			subset |= 1 << i;
		}
	}
	return (subset);
}

string value_formula(string const& formula, string const alphabet, uint32_t table_entry) {
	string valued_formula;
	char   values[26] = {0};

	for (size_t i = 0; i < alphabet.size(); ++i) {
		if (((table_entry >> (alphabet.size() - i - 1)) & 0x1) == 1) {
			values[i] = '1';
		} else {
			values[i] = '0';
		}
	}

	for (auto symbol : formula) {
		size_t pos = alphabet.find(symbol);

		if (pos == string::npos) {
			valued_formula += symbol;
		} else {
			valued_formula += values[pos];
		}
	}
	return (valued_formula);
}

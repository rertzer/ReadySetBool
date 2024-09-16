#include "print_truth_table.hpp"

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

void print_truth_table(string const& formula) {
	string	 formula_alphabet = get_alphabet(formula);
	uint32_t nb_of_table_entries = power(2, formula_alphabet.size());

	print_table_header(formula_alphabet);

	for (uint32_t table_entry = 0; table_entry < nb_of_table_entries; ++table_entry) {
		string valued_formula = value_formula(formula, formula_alphabet, table_entry);
		bool   result = eval_formula(valued_formula);
		print_table_entry(table_entry, formula_alphabet.size(), result);
	}
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

void print_table_header(string const& alphabet) {
	cout << "|";

	for (auto letter : alphabet) {
		cout << " " << letter << " |";
	}
	cout << " = |\n";
}

void print_table_entry(uint32_t table_entry, uint32_t alphabet_size, bool result) {
	char value;

	cout << "|";
	for (size_t i = 0; i < alphabet_size; ++i) {
		if (((table_entry >> (alphabet_size - i - 1)) & 0x1) == 1) {
			value = '1';
		} else {
			value = '0';
		}
		cout << " " << value << " |";
	}

	if (result) {
		cout << " 1 |\n";
	} else {
		cout << " 0 |\n";
	}
}

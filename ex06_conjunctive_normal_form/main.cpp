#include <vector>
#include "cnf.hpp"

int main() {
	vector<pair<string, string>> rpolish;
	rpolish.push_back(make_pair("AB|!", "A!B!&"));
	rpolish.push_back(make_pair("AB&!", "A!B!|"));
	rpolish.push_back(make_pair("FG=", "F!G|G!F|&"));
	rpolish.push_back(make_pair("AB=!!", "A!B|B!A|&"));
	rpolish.push_back(make_pair("A!!!!B!!!^", "A!B|AB!|&"));
	rpolish.push_back(make_pair("A!!!!!!B!!!!!^", "A!B|AB!|&"));
	rpolish.push_back(make_pair("AB>", "A!B|"));
	rpolish.push_back(make_pair("AB^", "A!B!|AB|&"));
	rpolish.push_back(make_pair("ABCD&|&", ""));
	rpolish.push_back(make_pair("", ""));
	rpolish.push_back(make_pair("AB|C&!", ""));
	rpolish.push_back(make_pair("AB|!", "A!B!&"));
	rpolish.push_back(make_pair("AB|C&", "AB|C&"));
	rpolish.push_back(make_pair("AB|C|D|", "ABCD|||"));
	rpolish.push_back(make_pair("AB&C&D&", "ABCD&&&"));
	rpolish.push_back(make_pair("AB&!C!|", "A!B!C!||"));
	rpolish.push_back(make_pair("AB|!C!&", "A!B!C!&&"));
	rpolish.push_back(make_pair("A!B!|!C&", ""));

	for (auto rp : rpolish) {
		try {
			cout << "rpolish:  " << rp.first << endl;
			string cnf = conjunctive_normal_form(rp.first);
			cout << "computed: " << cnf << endl;
			cout << "expected: " << rp.second << endl << endl << endl;
		} catch (const exception& e) {
			cerr << e.what() << endl << endl;
		}
	}
}

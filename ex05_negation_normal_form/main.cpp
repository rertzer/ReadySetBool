#include <vector>
#include "nnf.hpp"

int main() {
	vector<string> rpolish;
	rpolish.push_back("||");
	rpolish.push_back("F>ABC&|");
	rpolish.push_back("A");
	rpolish.push_back("A!");
	rpolish.push_back("AB&!");
	rpolish.push_back("AB|!");
	rpolish.push_back("AB>!");
	rpolish.push_back("AB=!");
	rpolish.push_back("ABC||");
	rpolish.push_back("ABC||!");
	rpolish.push_back("ABC|&");
	rpolish.push_back("ABC&|");
	rpolish.push_back("ABC&|!");
	rpolish.push_back("ABC^^");
	rpolish.push_back("ABC>>");
	rpolish.push_back("AB|C&!");
	rpolish.push_back("FG=");
	rpolish.push_back("AB=!!");
	rpolish.push_back("A!!!!B!!!^");
	rpolish.push_back("A!!!!!!B!!!!!^");
	rpolish.push_back("AB>");
	rpolish.push_back("");
	rpolish.push_back("AB^");
	rpolish.push_back("ABCD&|&");

	for (auto rp : rpolish) {
		cout << "rpolish: " << rp << endl;
		try {
			string reverted = negation_normal_form(rp);
			cout << reverted << "\n\n";
		} catch (const exception& e) {
			cerr << e.what() << "\n\n";
		}
	}
}

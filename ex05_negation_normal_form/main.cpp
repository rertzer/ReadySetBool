#include <vector>
#include "Formula.hpp"

int main() {
	vector<string> rpolish;
	rpolish.push_back("AB|C&!");
	rpolish.push_back("AB|!");
	rpolish.push_back("AB&!");
	rpolish.push_back("FG=");
	rpolish.push_back("AB=!!");
	rpolish.push_back("A!!!!B!!!^");
	rpolish.push_back("A!!!!!!B!!!!!^");
	rpolish.push_back("AB>");
	rpolish.push_back("");
	rpolish.push_back("AB^");
	for (auto rp : rpolish) {
		cout << "rpolish: " << rp << endl;
		try {
			Formula nnf(rp);
			nnf.print();
			string reverted = nnf.revertPolish();
			cout << reverted << endl;
			nnf.rewrite();
			cout << "double neg removed\n";
			nnf.print();
			reverted = nnf.revertPolish();
			cout << reverted << endl << endl;
		} catch (const exception& e) {
			cerr << e.what() << endl;
		}
	}
}

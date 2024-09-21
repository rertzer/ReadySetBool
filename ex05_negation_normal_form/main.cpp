#include "Formula.hpp"

int main() {
	string rpolish = "AB|C&!";

	try {
		Formula nnf(rpolish);
		cout << "rpolish: " << rpolish << endl;

		nnf.print();
	} catch (const exception& e) {
		cerr << e.what() << endl;
	}
}

#include "nnf.hpp"

string negation_normal_form(string const& rp) {
	Formula nnf(rp);
	// nnf.print();
	// cout << nnf.revertPolish() << endl;
	nnf.rewrite();
	// nnf.print();
	return (nnf.revertPolish());
}

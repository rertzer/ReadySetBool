#include "nnf.hpp"
#include <memory>
#include "Formula.hpp"

string negation_normal_form(string const& rp) {
	shared_ptr<Formula> nnf = make_shared<Formula>();
	nnf->fromString(rp);
	string nnf_rp;
	try {
		nnf->cnf();
		nnf_rp = nnf->revertPolish();
	} catch (const exception& e) {
		cerr << e.what() << endl << endl;
	}
	nnf.reset();

	return (nnf_rp);
}

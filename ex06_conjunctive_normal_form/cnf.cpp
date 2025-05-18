#include "cnf.hpp"
#include <memory>
#include "Formula.hpp"

string conjunctive_normal_form(string const& rp) {
	shared_ptr<Formula> cnf = make_shared<Formula>();
	cnf->fromString(rp);
	string cnf_rp;
	try {
		cnf->cnf();
		cnf_rp = cnf->revertPolish();
	} catch (const exception& e) {
		cerr << e.what() << endl << endl;
	}
	cnf.reset();

	return (cnf_rp);
}

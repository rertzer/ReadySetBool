#include "cnf.hpp"

string conjunctive_normal_form(string const& rp) {
	Formula cnf(rp);
	cnf.cnf();
	string cnf_rp = cnf.revertPolish();
	return (cnf_rp);
}

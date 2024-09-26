#ifndef POWERSET_HPP
#define POWERSET_HPP

#include <iostream>
#include <vector>

using namespace std;

class SizeException : public exception {
   public:
	virtual const char* what() const throw() { return ("Powerset error: set is to big"); }
};

vector<vector<int32_t>> powerset(vector<int32_t>);
#endif

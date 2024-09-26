#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <iostream>
#include <vector>

using namespace std;

template <class T>
ostream& operator<<(ostream& out, vector<T>& vec) {
	out << '{';
	for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
		if (it != vec.cbegin()) {
			out << ", ";
		}
		out << *it;
	}
	out << '}';

	return (out);
}

template <class T>
ostream& operator<<(ostream& out, vector<vector<T>>& vec) {
	out << '{';
	for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
		if (it != vec.cbegin()) {
			out << ", ";
		}
		out << '{';
		for (auto jt = it->cbegin(); jt != it->cend(); ++jt) {
			if (jt != it->cbegin()) {
				out << ", ";
			}
			out << *jt;
		}
		out << '}';
	}
	out << '}';

	return (out);
}
#endif

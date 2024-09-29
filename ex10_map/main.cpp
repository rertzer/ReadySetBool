/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:20:23 by rertzer           #+#    #+#             */
/*   Updated: 2024/09/29 14:20:33 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iomanip>
#include <iostream>
#include <sstream>
#include "map.hpp"

int main(int argc, char** argv) {
	if (argc != 3) {
		cerr << "usage: map number1 number2\n";
		return (1);
	}
	uint16_t x;
	uint16_t y;
	try {
		x = toInt(argv[1]);
		y = toInt(argv[2]);
	} catch (const exception& e) {
		cerr << e.what();
		return (1);
	}

	cout << "x: " << x << endl;
	cout << "y: " << y << endl;
	cout << setprecision(15) << "f: " << map(x, y) << endl;

	return (0);
}

uint16_t toInt(char* arg) {
	stringstream ss;
	ss << arg;
	uint16_t x;
	ss >> x;
	if (ss.fail()) {
		throw(InputException());
	}
	return (x);
}

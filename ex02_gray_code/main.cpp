/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 11:37:48 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 11:37:49 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bitset>
#include <iostream>
#include <sstream>
#include "gray_code.hpp"

using namespace std;

uint32_t readNumber(char* str) {
	uint32_t	 n;
	stringstream ss;

	ss.exceptions(ios::failbit);

	ss << str;
	ss >> n;

	return (n);
}

int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "adder require a number as arguments\n";
		return (1);
	}

	uint32_t a;

	try {
		a = readNumber(argv[1]);
	} catch (const exception& e) {
		cerr << "bad argument\n";
		return (1);
	}

	uint32_t c = gray_code(a);
	cout << "binary " << bitset<32>(a) << " (" << a << ")\n";
	cout << "gray   " << bitset<32>(c) << " (" << c << ")\n";

	return (0);
}

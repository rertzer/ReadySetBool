/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:55:20 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 14:55:22 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sat.hpp"

int main() {
	vector<string> formulas = {

		"AB&",	   "AB|",	  "AB>",	"BA^",	  "BA^AB^=",	   "BA^AA^=", "AB=",
		"ABAA||=", "AB|A&",	  "A!B|A&", "BB!|A&", "ABA|&",		   "ABA|&!",  "AAA",
		"|A=",	   "A23",	  "A+A=B",	"A",	  "AX|",		   "AA!&",	  "AA^",
		"AB>",	   "ABCD||=", "AB|C&",	"ABC|&",  "ABCDEFG||||||",
		//"ABCDEFGHIJKLMNOPQRSTUVWXYZ|||||||||||||||||||||||||"};

	};

	for (auto form : formulas) {
		try {
			cout << form << "\t" << boolalpha << sat(form) << "\n";
		} catch (RPNException& e) {
			cout << e.what() << endl;
		}
	}
	return (0);
}

/*
 * formula	-h						evaluation
 * 10&		1 and 0						0
 * 10|		1 or 0						1
 * 11>		1 => 1						1
 * 01^		0 xor 1						1
 * 01^10^=	(0 xor 1) == (1 xor 0)		1
 * 01^11^=	(0 xor 1) == (1 xor 1)		0
 * 10=		1 == 0						0
 * 1011||=	((1 or 1) or 0) == 1		1
 * 10|1&	(1 or 0) and 1				1
 * 1!0|1&	(not 1 or 0) and 1			0
 * 00!|1&	(0 or not 0) and 1			1
 * 101|&	1 and (1 or 0)				1
 * 101|&!	1 and (1 or 0)				0
 */

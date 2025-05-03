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

#include <iostream>
#include <vector>
#include "eval_formula.hpp"

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

int main() {
	vector<pair<string, string>> formulas = {
		{"10&", "0"},	  {"10|", "1"},		  {"11>", "1"},		{"01^", "1"},	  {"01^10^=", "1"},
		{"01^11^=", "0"}, {"10=", "0"},		  {"1011||=", "1"}, {"10|1&", "1"},	  {"1!0|1&", "0"},
		{"00!|1&", "1"},  {"101|&", "1"},	  {"101|&!", "0"},	{"111", "Error"}, {"|1=", "Error"},
		{"123", "Error"}, {"1+1=0", "Error"}, {"1", "1"}};

	for (auto form : formulas) {
		try {
			bool result = eval_formula(form.first);
			cout << "formula: " << form.first << "\nevaluation: " << result
				 << " (expected: " << form.second << ")\n\n";
		} catch (RPNException& e) {
			cout << form.first << "\n" << e.what() << " (expected: " << form.second << ")\n\n";
		}
	}
	return (0);
}

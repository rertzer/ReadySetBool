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

#include "print_truth_table.hpp"

int main() {
	vector<string> formulas = {"AB&",
							   "AB|",
							   "AB>",
							   "AB=",
							   "AB^",
							   "ABCD||=",
							   "ABCD||=!",
							   "AB|C&",
							   "ABC|&",
							   "A!B!C!D!EFG||||||",
							   "DE^AD|&B^EE!|&F=",
							   "ABCDEFGHIJKLMNOPQRSTUVWXYZ|||||||||||||||||||||||||"};

	for (auto form : formulas) {
		cout << form << "\n";
		try {
			print_truth_table(form);
		} catch (PTTException& e) {
			cout << "Oops! " << e.what();
		} catch (RPNException& e) {
			cout << "Oops! " << e.what();
		}
		cout << "\n";
	}
	return (0);
}

/*
 *	DE^AD|&B^EE!|&F=
 *	((((D^E)&(A|D))^B)&(E|!E)) = F
 *	E|!E always true, simplify as:
 *	(((D^E)&(A|D))^B) = F
 */

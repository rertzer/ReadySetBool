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
		"AB|", "AB&", "AA!&", "AA^", "AB>", "AB=", "ABCD||=", "AB|C&", "ABC|&", "ABCDEFG||||||",
		//"ABCDEFGHIJKLMNOPQRSTUVWXYZ|||||||||||||||||||||||||"
	};

	for (auto form : formulas) {
		cout << form << "\t" << boolalpha << sat(form) << "\n";
	}
	return (0);
}

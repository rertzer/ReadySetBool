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

#include "eval_set.hpp"
#include "printer.hpp"

int main() {
	vector<string> formulas = {
		"AB&", "AB|", "A!"	//, "AB>", "AB=", "ABCD||=", "AB|C&", "ABC|&", "ABCDEFG||||||",
							//"ABCDEFGHIJKLMNOPQRSTUVWXYZ|||||||||||||||||||||||||"
	};
	vector<vector<vector<int32_t>>> sets = {
		{{0, 1, 2}, {0, 3, 4}}, {{0, 1, 2}, {3, 4, 5}}, {{0, 1, 2}}};

	for (size_t i = 0; i < formulas.size(); ++i) {
		vector<int32_t> soluce = eval_set(formulas[i], sets[i]);
		cout << formulas[i] << "\t" << soluce << "\n";
	}
	return (0);
}

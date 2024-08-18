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

#include "eval_formula.hpp"

int	main()
{
	vector<string>	formulas = {
		"10&",
		"10|",
		"11>",
		"10=",
		"1011||=",
		"10|1&",
		"101|&"
	};

	for	(auto form : formulas)
	{
		bool	result = eval_formula(form);
		cout << form << "\n" << result << "\n";
	}
	return (0);
}

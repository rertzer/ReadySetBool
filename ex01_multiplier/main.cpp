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

#include "multiplier.hpp"
#include <sstream>
#include <bitset>

uint32_t	readNumber(char *str)
{
	uint32_t		n;
	stringstream	ss;

	ss.exceptions(ios::failbit);
	
	ss << str;
	ss >> n;

	return (n);
}

int	main(int argc, char **argv)
{

	if (argc != 3)
	{
		cerr << "multiplier require two numbers as arguments\n";
		return (1);
	}
	
	uint32_t	a;
	uint32_t	b;

	try
	{
		a = readNumber(argv[1]);
		b = readNumber(argv[2]);
	}
	catch (const exception& e)
	{
		cerr << "bad argument\n";
		return (1);
	}
	
	uint32_t	c = multiplier(a, b);

	cout << "   " << bitset<32>(a) << " (" << a << ")\n";
	cout << " * " << bitset<32>(b) << " (" << b  << ")\n";
	cout << "__________________________________\n";
	cout << "   " << bitset<32>(c) << " (" << c << ")\n";

	return (0);
}

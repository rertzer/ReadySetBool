/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adder.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 11:12:35 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/21 12:52:59 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "adder.hpp"

static inline bool	overflow(uint32_t a, uint32_t b);

uint32_t	adder(uint32_t a, uint32_t b)
{
	uint32_t	sum = 0;
	uint32_t	carry = 0;
	bool		over = overflow(a, b);

	
	sum = a ^ b;
	carry = (a & b) << 1;

	while (carry != 0)	
	{
		if (overflow(sum, carry))
		{
			over = true;
		}
		uint32_t	tmp = sum;
		sum = tmp ^ carry;
		carry = (tmp & carry) << 1;
		
	}
	if (over == true)
	{
		cerr << "Warning: overflow\n";
	}

	return (sum);
}

static inline bool	overflow(uint32_t a, uint32_t b)
{
	bool	over = false;

	if ((a & 0x80000000) && (b & 0x80000000))
	{
		over = true;
	}

	return (over);
}

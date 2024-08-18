/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adder.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 11:12:35 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 11:12:37 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "adder.hpp"

uint32_t	adder(uint32_t a, uint32_t b)
{
	uint32_t	sum = 0;
	uint32_t	carry = 0;

	sum = a ^ b;
	carry = (a & b) << 1;

	while (carry != 0)	
	{
		uint32_t	tmp = sum;
		sum = tmp ^ carry;
		carry = (tmp & carry) << 1;
	}

	return (sum);
}

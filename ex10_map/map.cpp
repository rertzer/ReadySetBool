/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:21:21 by rertzer           #+#    #+#             */
/*   Updated: 2024/09/29 14:21:24 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.hpp"

double map(uint16_t x, uint16_t y) {
	uint32_t a = 0;

	for (size_t i = 0; i < 16; ++i) {
		a |= (x & (1 << i)) << i;
		a |= (y & (1 << i)) << (i + 1);
	}
	double f = static_cast<double>(a) / static_cast<double>((1LL << 32) - 1);

	return (f);
}

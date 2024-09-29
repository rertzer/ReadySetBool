/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_map.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:04:01 by rertzer           #+#    #+#             */
/*   Updated: 2024/09/29 16:04:06 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reverse_map.hpp"

point reverse_map(double f) {
	uint32_t xy = 0;
	point	 p = {0, 0};

	f *= ((1LL << 32) - 1);
	xy = static_cast<uint32_t>(f);
	for (uint16_t i = 0; i < 16; ++i) {
		p.x |= static_cast<uint16_t>((xy & (1 << (2 * i))) >> i);
		p.y |= static_cast<uint16_t>((xy & (1 << (2 * i + 1))) >> (i + 1));
	}

	return (p);
}

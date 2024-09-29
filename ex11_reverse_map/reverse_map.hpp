/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_map.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:03:19 by rertzer           #+#    #+#             */
/*   Updated: 2024/09/29 16:03:24 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REVERSE_MAP
#define REVERSE_MAP

#include "map.hpp"

struct point {
	uint16_t x;
	uint16_t y;
};

point reverse_map(double f);
#endif

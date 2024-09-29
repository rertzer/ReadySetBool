/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:22:15 by rertzer           #+#    #+#             */
/*   Updated: 2024/09/29 14:22:26 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
#define MAP_HPP

#include <stdint.h>
#include <cstddef>
#include <exception>

using namespace std;

double	 map(uint16_t x, uint16_t y);
uint16_t toInt(char* arg);

class InputException : public exception {
   public:
	virtual const char* what() const throw() { return ("Invalid argument"); }
};

#endif

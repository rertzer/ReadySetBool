/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiplier.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 13:04:55 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 13:04:57 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "multiplier.hpp"

uint32_t multiplier(uint32_t a, uint32_t b) {
  uint32_t result = 0;
  bool over = false;

  for (int shift = 0; shift < 32; ++shift) {
    if ((b >> shift) & 1) {
      result = adder(result, a << shift, &over);
    }
  }
  if (over == true) {
    cerr << "Warning: overflow\n";
  }

  return (result);
}

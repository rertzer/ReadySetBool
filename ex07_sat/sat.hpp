/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:13:03 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 14:13:04 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SAT_HPP
#define SAT_HPP

#include <deque>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>

#include "eval_formula.hpp"

using namespace std;

bool   sat(string const& formula);
string get_alphabet(string const& formula);
string value_formula(string const& formula, string const alphabet, uint32_t table_entry);

class PTTException : public std::exception {
   public:
	virtual const char* what() const throw() { return "Error"; }
};

#endif

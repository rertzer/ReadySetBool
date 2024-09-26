/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_set.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:13:03 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 14:13:04 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_SET_HPP
#define EVAL_SET_HPP

#include <algorithm>
#include <deque>
#include <exception>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

#include "eval_formula.hpp"

using namespace std;

vector<int32_t> eval_set(string const& formula, vector<vector<int32_t>> sets);
set<int32_t>	get_union_set(vector<vector<int32_t>> const& sets);
uint32_t		get_subset(vector<vector<int32_t>> sets, int32_t number);
bool			eval_number(string const& formula, string const alphabet, uint32_t subset);
string			get_alphabet(string const& formula);
string			value_formula(string const& formula, string const alphabet, uint32_t subset);

class PTTException : public std::exception {
   public:
	virtual const char* what() const throw() { return "Error"; }
};

#endif

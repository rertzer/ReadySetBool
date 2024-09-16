/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_formula.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:12:49 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 14:12:50 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval_formula.hpp"

bool eval_formula(std::string exp) {
	std::deque<bool> filo;
	opfun			 op = nullptr;

	for (size_t i = 0; i < exp.length(); i++) {
		if (isValue(exp[i])) {
			filo.push_front(toBool(exp[i]));
		} else if (isNeg(exp[i])) {
			if (filo.size() < 1)
				throw(RPNException());
			bool val = filo.front();
			filo.pop_front();
			filo.push_front(func_neg(val));
		} else if (!isspace(exp[i])) {
			op = getop(exp[i]);
			if (op) {
				if (filo.size() < 2)
					throw(RPNException());
				bool rhs = filo.front();
				filo.pop_front();
				bool lhs = filo.front();
				filo.pop_front();
				filo.push_front(op(lhs, rhs));
			} else
				throw(RPNException());
		}
	}
	if (filo.size() != 1)
		throw(RPNException());
	return (filo.front());
}

opfun getop(char c) {
	char  symbols[] = {'&', '|', '^', '>', '='};
	opfun operations[] = {&func_and, &func_or, &func_xor, &func_imply, &func_equal};

	for (size_t i = 0; i < 5; ++i) {
		if (c == symbols[i])
			return (operations[i]);
	}
	return (0);
}

bool isValue(char c) {
	if (c == '0' || c == '1')
		return (true);
	return (false);
}

bool isNeg(char c) {
	if (c == '!')
		return (true);
	return (false);
}

bool toBool(char c) {
	if (c == '1')
		return (true);
	return (false);
}

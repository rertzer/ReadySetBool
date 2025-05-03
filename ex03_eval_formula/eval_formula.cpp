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
#include <deque>

static void handleValue(deque<bool>& filo, char c);
static void handleNeg(deque<bool>& filo);
static void handleOp(deque<bool>& filo, char c);

using namespace std;

bool eval_formula(string exp) {
	deque<bool> filo;

	for (size_t i = 0; i < exp.length(); ++i) {
		if (isValue(exp[i])) {
			handleValue(filo, exp[i]);
		} else if (isNeg(exp[i])) {
			handleNeg(filo);
		} else if (!isspace(exp[i])) {
			handleOp(filo, exp[i]);
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
	return (nullptr);
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

static void handleValue(deque<bool>& filo, char c) {
	filo.push_front(toBool(c));
}

static void handleNeg(deque<bool>& filo) {
	if (filo.size() < 1)
		throw(RPNException());
	bool val = filo.front();
	filo.pop_front();
	filo.push_front(func_neg(val));
}

static void handleOp(deque<bool>& filo, char c) {
	opfun op = nullptr;
	op = getop(c);
	if (op != nullptr) {
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

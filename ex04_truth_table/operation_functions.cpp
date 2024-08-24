/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_functions.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:37:26 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 14:37:27 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval_formula.hpp"

bool	func_neg(bool a)
{
	return (!a);
}

bool	func_and(bool a, bool b)
{
	return (a && b);
}

bool	func_or(bool a, bool b)
{
	return (a || b);
}

bool	func_xor(bool a, bool b)
{
	return (a != b);
}

bool	func_imply(bool a, bool b)
{
	return (!a || b);
}

bool	func_equal(bool a, bool b)
{
	return (a == b);
}

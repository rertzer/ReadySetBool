/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_formula.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:13:03 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 14:13:04 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_FORMULA_HPP
# define EVAL_FORMULA_HPP

# include <iostream>
# include <sstream>
# include <deque>
# include <vector>
# include <exception>

using namespace std;

typedef	bool	(*opfun)(bool, bool);


//eval_formula
bool	eval_formula(std::string exp);
opfun	getop(char c);
bool	isValue(char c);
bool	isNeg(char c);
bool	toBool(char c);

//operation_functions
bool	func_neg(bool a);
bool	func_and(bool a, bool b);
bool	func_or(bool a, bool b);
bool	func_xor(bool a, bool b);
bool	func_imply(bool a, bool b);
bool	func_equal(bool a, bool b);



class	RPNException: public std::exception
{
	public:
		virtual const char * what() const throw ()
		{
			return  "Error";
		}
};

#endif

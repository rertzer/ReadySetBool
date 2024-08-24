/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_truth_table.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rertzer <rertzer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:13:03 by rertzer           #+#    #+#             */
/*   Updated: 2024/08/18 14:13:04 by rertzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_TRUTH_TABLE_HPP
# define PRINT_TRUTH_TABLE_HPP

# include <iostream>
# include <sstream>
# include <deque>
# include <vector>
# include <exception>

# include "eval_formula.hpp"

using namespace std;


void	print_truth_table(string const & formula);
string	get_alphabet(string const & formula);
string	value_formula(string const & formula, string const alphabet, uint32_t table_entry);
void	print_table_header(string const & alphabet);
void	print_table_entry(uint32_t table_entry, uint32_t alphabet_size, bool result);

class	PTTException: public std::exception
{
	public:
		virtual const char * what() const throw ()
		{
			return  "Error";
		}
};

#endif

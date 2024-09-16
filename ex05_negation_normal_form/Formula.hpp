#ifndef FORMULA_HPP
#define FORMULA_HPP

#include <iostream>
#include <string>

using namespace std;

// root (empty place holder), negation, operator, variable
enum class Kind { Root, Neg, Op, Var };
// non applicable, conjunction, disjunction, exclusive disjunction, material condition, logical
// equivalence
enum class Op { Na, Conj, Dis, Edis, Mcond, Leq };

class Formula {
   public:
	Formula();
	Formula(string rp);
	Formula(Formula const& f);
	Formula(Formula&& f);
	~Formula();
	Formula& operator=(Formula const& f);
	Formula& operator=(Formula&& f);

	void   erase();
	void   fromString(string reversePolish);
	string reversePolish() const;
	void   nnf();

   private:
	void	 addChildsFromString(string rp);
	Kind	 extractKind(string rp);
	Kind	 kind;
	Op		 op;
	string	 name;
	Formula* parent;
	Formula* left_child;
	Formula* right_child;
};

#endif	// !FORMULA_HPP

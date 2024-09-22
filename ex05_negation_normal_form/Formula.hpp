#ifndef FORMULA_HPP
#define FORMULA_HPP
#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include "SuperStack.hpp"

using namespace std;

// root (empty place holder), negation, operator, variable
enum class Kind { Root, Neg, Op, Var };
// non applicable, conjunction, disjunction, exclusive disjunction, material condition, logical
// equivalence
enum class Op { Na, Conj, Dis, Edis, Mcond, Leq };

enum class Visit { First, Second, Third };

class Formula {
   public:
	Formula();
	Formula(string rp);
	Formula(char s);
	Formula(Formula const& f);
	Formula(Formula&& f);
	~Formula();
	Formula& operator=(Formula const& f);
	Formula& operator=(Formula&& f);

	void   rewrite();
	void   erase();
	void   fromString(string& reversePolish);
	string revertPolish() const;
	void   removeDoubleNeg();
	void   nnf();
	void   print();
	char   getSymbol() const;
	char   getOpSymbol() const;

   private:
	class InternalException : public exception {
	   public:
		virtual const char* what() const throw() { return ("Formula error: internal error"); }
	};
	class InvalidStringException : public exception {
	   public:
		virtual const char* what() const throw() {
			return ("Formula error: invalid reverse polish string");
		}
	};

	Formula* addOp(string& rp);
	Formula* addNeg(string& rp);
	Formula* addVar(string& rp);
	void	 addChildsFromString(string& rp, SuperStack<Formula*>& to_visit);
	void	 addChildToRoot(string& rp, SuperStack<Formula*>& to_visit);
	void	 addChildsToOp(string& rp, SuperStack<Formula*>& to_visit);
	void	 addChildToNeg(string& rp, SuperStack<Formula*>& to_visit);
	char	 extractNextSymbol(string& rp);
	Kind	 charToKind(char s);
	Op		 charToOp(char s);
	char	 charToName(char s);
	void	 removeDoubleNegNode(SuperStack<Formula*>& to_visit);
	Formula* removeDoubleNegChild(SuperStack<Formula*>& to_visit, Formula* child);
	void	 rewriteDoubleNegation();
	void	 rewriteMaterialCondition();
	void	 rewriteEquivalence();
	void	 rewriteExclusiveDisjunction();
	void	 revertNode(string& rp, SuperStack<Formula const*>& to_reverse) const;
	void	 revertRoot(SuperStack<Formula const*>& to_reverse) const;
	void	 revertOp(string& rp, SuperStack<Formula const*>& to_reverse) const;
	void	 revertNeg(string& rp, SuperStack<Formula const*>& to_reverse) const;
	void	 revertVar(string& rp) const;
	void	 printNode(SuperStack<Formula*>& to_visit);
	void	 printRoot(SuperStack<Formula*>& to_visit);
	void	 printOp(SuperStack<Formula*>& to_visit);
	void	 printNeg(SuperStack<Formula*>& to_visit);
	void	 printVar();
	void	 printSymbol();
	Kind	 kind;
	Op		 op;
	char	 name;
	Visit	 visited;
	Formula* parent;
	Formula* left_child;
	Formula* right_child;
};

#endif	// !FORMULA_HPP

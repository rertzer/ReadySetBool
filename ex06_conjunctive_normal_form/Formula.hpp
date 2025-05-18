#ifndef FORMULA_HPP
#define FORMULA_HPP
#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
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

class Formula : public enable_shared_from_this<Formula> {
   public:
	Formula();
	Formula(char s);
	Formula(Formula const& f);
	Formula(Formula&& f);
	Formula(shared_ptr<Formula> f);
	~Formula();
	Formula& operator=(Formula const& f);
	Formula& operator=(Formula&& f);

	void   kill();
	void   rewrite();
	void   erase();
	void   fromString(string reversePolish);
	string revertPolish();
	void   nnf();
	void   cnf();
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

	shared_ptr<Formula> addOp(string& rp);
	shared_ptr<Formula> addNeg(string& rp);
	shared_ptr<Formula> addVar(string& rp);
	void				addChildsFromString(string& rp, SuperStack<shared_ptr<Formula>>& to_visit);
	void				addChildToRoot(string& rp, SuperStack<shared_ptr<Formula>>& to_visit);
	void				addChildsToOp(string& rp, SuperStack<shared_ptr<Formula>>& to_visit);
	void				addChildToNeg(string& rp, SuperStack<shared_ptr<Formula>>& to_visit);
	char				extractNextSymbol(string& rp);
	Kind				charToKind(char s);
	Op					charToOp(char s);
	char				charToName(char s);
	void				rewriteNode(SuperStack<weak_ptr<Formula>>& to_visit);
	shared_ptr<Formula> rewriteChild(SuperStack<weak_ptr<Formula>>& to_visit,
									 shared_ptr<Formula>			child);
	void				rewriteDoubleNegNode(SuperStack<weak_ptr<Formula>>& to_visit);
	shared_ptr<Formula> rewriteDoubleNegation(SuperStack<weak_ptr<Formula>>& to_visit,
											  shared_ptr<Formula>			 child);
	void				rewriteMaterialCondition();
	void				rewriteEquivalence();
	void				rewriteExclusiveDisjunction();
	void				rewriteDisjunction(SuperStack<weak_ptr<Formula>>& to_visit);
	void				rewriteDisjunctionLeft();
	void				rewriteDisjunctionRight();
	shared_ptr<Formula> rewriteMorganConj(SuperStack<weak_ptr<Formula>>& to_visit,
										  shared_ptr<Formula>			 child);
	shared_ptr<Formula> rewriteMorganDis(SuperStack<weak_ptr<Formula>>& to_visit,
										 shared_ptr<Formula>			child);
	shared_ptr<Formula> negate();
	void  revertNode(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_reverse);
	void  revertRoot(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_reverse);
	void  revertOp(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_reverse);
	void  revertNeg(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_reverse);
	void  revertVar(string& rp);
	void  printNode(SuperStack<shared_ptr<Formula>>& to_visit);
	void  printRoot(SuperStack<shared_ptr<Formula>>& to_visit);
	void  printOp(SuperStack<shared_ptr<Formula>>& to_visit);
	void  printNeg(SuperStack<shared_ptr<Formula>>& to_visit);
	void  printVar();
	void  printSymbol();
	Kind  kind;
	Op	  op;
	char  name;
	Visit visited;
	shared_ptr<Formula> left_child;
	shared_ptr<Formula> right_child;
};

#endif	// !FORMULA_HPP

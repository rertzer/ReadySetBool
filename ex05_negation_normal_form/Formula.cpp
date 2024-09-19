#include "Formula.hpp"
#include <cctype>
#include <ios>

Formula::Formula()
	: kind(Kind::Root),
	  op(Op::Na),
	  name('\0'),
	  visited(Visit::First),
	  parent(nullptr),
	  left_child(nullptr),
	  right_child(nullptr) {}

Formula::Formula(string rp)
	: kind(Kind::Root),
	  op(Op::Na),
	  name('\0'),
	  visited(Visit::First),
	  parent(nullptr),
	  left_child(nullptr),
	  right_child(nullptr) {
	fromString(rp);
}

Formula::Formula(char s)
	: kind(charToKind(s)),
	  op(charToOp(s)),
	  name(charToName(s)),
	  visited(Visit::First),
	  parent(nullptr),
	  left_child(nullptr),
	  right_child(nullptr) {}

Formula::Formula(Formula const& f) {
	*this = f;
}

Formula::Formula(Formula&& f) {
	*this = f;
	f.parent = nullptr;
	f.left_child = nullptr;
	f.right_child = nullptr;
}

Formula::~Formula() {}

Formula& Formula::operator=(Formula const& f) {
	if (this != &f) {
		kind = f.kind;
		op = f.op;
		name = f.name;
		visited = f.visited;
		parent = f.parent;
		left_child = f.left_child;
		right_child = f.right_child;
	}
	return (*this);
}

Formula& Formula::operator=(Formula&& f) {
	*this = f;
	f.parent = nullptr;
	f.left_child = nullptr;
	f.right_child = nullptr;
	return (*this);
}

void Formula::erase() {
	if (left_child != nullptr) {
		left_child->erase();
	}
	if (right_child != nullptr) {
		right_child->erase();
	}
	if (kind != Kind::Root)
		this->~Formula();
}

void Formula::fromString(string rp) {
	if (kind != Kind::Root)
		throw(InternalException());
	SuperStack<Formula*> to_visit;
	to_visit.push(this);
	while (!rp.empty()) {
		addChildsFromString(rp, to_visit);
	}
	if (!to_visit.empty())
		throw(InvalidStringException());
}

void Formula::addChildsFromString(string& rp, SuperStack<Formula*>& to_visit) {
	Formula* current_node = to_visit.popout();

	switch (current_node->kind) {
		case Kind::Root:
			addChildToRoot(rp, to_visit);
			break;
		case Kind::Op:
			addChildsToOp(rp, to_visit);
			break;
		case Kind::Neg:
			addChildToNeg(rp, to_visit);
			break;
		default:
			throw(InvalidStringException());
			break;
	}
}

void Formula::addChildToRoot(string& rp, SuperStack<Formula*>& to_visit) {
	char s = extractNextSymbol(rp);
	left_child = new Formula(s);
	if (left_child->kind != Kind::Var)
		to_visit.push(left_child);
}

void Formula::addChildsToOp(string& rp, SuperStack<Formula*>& to_visit) {
	char s = extractNextSymbol(rp);
	left_child = new Formula(s);
	if (left_child->kind != Kind::Var)
		to_visit.push(left_child);
	s = extractNextSymbol(rp);
	right_child = new Formula(s);
	if (right_child->kind != Kind::Var)
		to_visit.push(right_child);
}

void Formula::addChildToNeg(string& rp, SuperStack<Formula*>& to_visit) {
	char s = extractNextSymbol(rp);
	left_child = new Formula(s);
	if (left_child->kind != Kind::Var)
		to_visit.push(left_child);
}

char Formula::extractNextSymbol(string& rp) {
	if (rp.empty())
		throw(InvalidStringException());
	char s = rp.back();
	rp.pop_back();
	return (s);
}

Kind Formula::charToKind(char k) {
	Kind kind = Kind::Root;

	switch (k) {
		case '!':
			kind = Kind::Neg;
			break;
		case '&':
		case '|':
		case '^':
		case '>':
		case '=':
			kind = Kind::Op;
			break;
		default:
			if (isupper(k))
				kind = Kind::Var;
			else
				throw(InvalidStringException());
			break;
	}
	return (kind);
}

Op Formula::charToOp(char s) {
	Op op;
	switch (s) {
		case '&':
			op = Op::Conj;
			break;
		case '|':
			op = Op::Dis;
			break;
		case '^':
			op = Op::Edis;
			break;
		case '>':
			op = Op::Mcond;
			break;
		case '=':
			op = Op::Leq;
			break;
		default:
			op = Op::Na;
	}
	return (op);
}

char Formula::charToName(char s) {
	char n;
	if (isupper(s))
		n = s;
	else
		n = '\0';
	return (n);
}

void Formula::print() {
	SuperStack<Formula*> to_visit;
	to_visit.push(this);

	while (!to_visit.empty()) {
		Formula* node = to_visit.popout();
		node->printNode(to_visit);
	}
}

void Formula::printNode(SuperStack<Formula*>& to_visit) {
	switch (kind) {
		case Kind::Root:
			break;
		case Kind::Op:
			printOp(to_visit);
			break;
		case Kind::Neg:
			printNeg(to_visit);
			break;
		case Kind::Var:
			printVar(to_visit);
			break;
	}
}

void Formula::printOp(SuperStack<Formula*>& to_visit) {
	if (visited == Visit::First) {
		cout << '(';
		to_visit.push(left_child);
		to_visit.push(this);
		to_visit.push(right_child);
		to_visit.push(this);
		visited = Visit::Second;
	} else if (visited == Visit::Second) {
		printSymbol();
		visited = Visit::Third;
	}
}

void Formula::printNeg(SuperStack<Formula*>& to_visit) {}

void Formula::printVar(SuperStack<Formula*>& to_visit) {}

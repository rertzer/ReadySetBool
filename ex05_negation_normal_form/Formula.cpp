#include "Formula.hpp"
#include "SuperStack.hpp"

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

void Formula::rewrite() {
	SuperStack<Formula*> to_visit;
	to_visit.push(this);
	while (!to_visit.empty()) {
		rewriteNode(to_visit);
	}
}

void Formula::rewriteNode(SuperStack<Formula*>& to_visit) {
	Formula* current_node = to_visit.popout();

	if (current_node->left_child != nullptr) {
		current_node->left_child = rewriteChild(to_visit, current_node->left_child);
	}
	if (current_node->right_child != nullptr) {
		current_node->right_child = rewriteChild(to_visit, current_node->right_child);
	}
}

Formula* Formula::rewriteChild(SuperStack<Formula*>& to_visit, Formula* child) {
	Formula* kid = child;
	if (child->kind == Kind::Neg) {
		if (child->left_child->kind == Kind::Neg) {
			kid = rewriteDoubleNegation(to_visit, child);
		} else if (child->left_child->op == Op::Conj) {
			kid = rewriteMorganConj(to_visit, child);
		} else if (child->left_child->op == Op::Dis) {
			kid = rewriteMorganDis(to_visit, child);
		} else {
			to_visit.push(child);
		}

	} else if (child->kind == Kind::Op) {
		switch (child->op) {
			case Op::Mcond:
				child->rewriteMaterialCondition(to_visit);
				break;
			case Op::Leq:
				child->rewriteEquivalence(to_visit);
				break;
			case Op::Edis:
				child->rewriteExclusiveDisjunction(to_visit);
				break;
			default:
				to_visit.push(child);
				break;
		}
	} else {
		to_visit.push(child);
	}

	return (kid);
}

Formula* Formula::rewriteMorganConj(SuperStack<Formula*>& to_visit, Formula* child) {
	Formula* kid = child->left_child;
	child->~Formula();
	kid->op = Op::Dis;
	kid->left_child = kid->left_child->negate();
	kid->right_child = kid->right_child->negate();
	to_visit.push(this);

	return (kid);
}

Formula* Formula::rewriteMorganDis(SuperStack<Formula*>& to_visit, Formula* child) {
	Formula* kid = child->left_child;
	child->~Formula();
	kid->op = Op::Conj;
	kid->left_child = kid->left_child->negate();
	kid->right_child = kid->right_child->negate();
	to_visit.push(this);

	return (kid);
}

Formula* Formula::rewriteDoubleNegation(SuperStack<Formula*>& to_visit, Formula* child) {
	Formula* kid = child->left_child->left_child;
	child->left_child->~Formula();
	child->~Formula();
	to_visit.push(this);

	return (kid);
}

void Formula::rewriteMaterialCondition(SuperStack<Formula*>& to_visit) {
	Formula* kid = left_child;
	left_child = new Formula('!');
	left_child->left_child = kid;
	op = Op::Dis;
	to_visit.push(this);
}

void Formula::rewriteEquivalence(SuperStack<Formula*>& to_visit) {
	Formula* left_kid = left_child;
	Formula* right_kid = right_child;

	left_child = new Formula('>');
	right_child = new Formula('>');

	left_child->left_child = left_kid;
	left_child->right_child = right_kid;
	right_child->left_child = right_kid;
	right_child->right_child = left_kid;

	op = Op::Conj;

	to_visit.push(this);
}

void Formula::rewriteExclusiveDisjunction(SuperStack<Formula*>& to_visit) {
	Formula* left_kid = left_child;
	Formula* right_kid = right_child;

	left_child = new Formula('&');
	right_child = new Formula('&');

	left_child->left_child = left_kid;
	left_child->right_child = right_kid->negate();
	right_child->left_child = left_kid->negate();
	right_child->right_child = right_kid;

	op = Op::Dis;

	to_visit.push(this);
}

Formula* Formula::negate() {
	Formula* neg = new Formula('!');
	neg->left_child = this;

	return (neg);
}

string Formula::revertPolish() const {
	SuperStack<Formula const*> to_visit;
	string					   rp;
	to_visit.push(this);
	while (!to_visit.empty()) {
		revertNode(rp, to_visit);
	}
	reverse(rp.begin(), rp.end());

	return (rp);
}

void Formula::revertNode(string& rp, SuperStack<Formula const*>& to_visit) const {
	Formula const* current_node = to_visit.popout();

	switch (current_node->kind) {
		case Kind::Root:
			current_node->revertRoot(to_visit);
			break;
		case Kind::Op:
			current_node->revertOp(rp, to_visit);
			break;
		case Kind::Neg:
			current_node->revertNeg(rp, to_visit);
			break;
		case Kind::Var:
			current_node->revertVar(rp);
			break;
	}
}

void Formula::revertRoot(SuperStack<Formula const*>& to_reverse) const {
	to_reverse.push(left_child);
}

void Formula::revertOp(string& rp, SuperStack<Formula const*>& to_reverse) const {
	char s = getSymbol();
	rp.push_back(s);
	to_reverse.push(left_child);
	to_reverse.push(right_child);
}

void Formula::revertNeg(string& rp, SuperStack<Formula const*>& to_reverse) const {
	char s = getSymbol();
	rp.push_back(s);
	to_reverse.push(left_child);
}

void Formula::revertVar(string& rp) const {
	char s = getSymbol();
	rp.push_back(s);
}

char Formula::getSymbol() const {
	char s;
	switch (kind) {
		case Kind::Op:
			s = getOpSymbol();
			break;
		case Kind::Neg:
			s = '!';
			break;
		case Kind::Var:
			s = name;
			break;
		case Kind::Root:
			throw(InternalException());
			break;
	}
	return s;
}

char Formula::getOpSymbol() const {
	char s;
	switch (op) {
		case Op::Dis:
			s = '|';
			break;
		case Op::Leq:
			s = '=';
			break;
		case Op::Conj:
			s = '&';
			break;
		case Op::Edis:
			s = '^';
			break;
		case Op::Mcond:
			s = '>';
			break;
		case Op::Na:
			throw(InternalException());
	}
	return (s);
}

void Formula::fromString(string& rp) {
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
			current_node->addChildToRoot(rp, to_visit);
			break;
		case Kind::Op:
			current_node->addChildsToOp(rp, to_visit);
			break;
		case Kind::Neg:
			current_node->addChildToNeg(rp, to_visit);
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
	if (visited == Visit::First) {
		char s = extractNextSymbol(rp);
		right_child = new Formula(s);
		to_visit.push(this);
		if (right_child->kind != Kind::Var)
			to_visit.push(right_child);
		visited = Visit::Second;
	} else if (visited == Visit::Second) {
		char s = extractNextSymbol(rp);
		left_child = new Formula(s);
		if (left_child->kind != Kind::Var)
			to_visit.push(left_child);
		visited = Visit::First;
	}
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
	cout << endl;
}

void Formula::printNode(SuperStack<Formula*>& to_visit) {
	switch (kind) {
		case Kind::Root:
			printRoot(to_visit);
			break;
		case Kind::Op:
			printOp(to_visit);
			break;
		case Kind::Neg:
			printNeg(to_visit);
			break;
		case Kind::Var:
			printVar();
			break;
	}
}

void Formula::printRoot(SuperStack<Formula*>& to_visit) {
	to_visit.push(left_child);
}

void Formula::printOp(SuperStack<Formula*>& to_visit) {
	if (visited == Visit::First) {
		cout << '(';
		to_visit.push(this);
		to_visit.push(right_child);
		to_visit.push(this);
		to_visit.push(left_child);
		visited = Visit::Second;
	} else if (visited == Visit::Second) {
		printSymbol();
		visited = Visit::Third;
	} else if (visited == Visit::Third) {
		cout << ')';
		visited = Visit::First;
	}
}

void Formula::printNeg(SuperStack<Formula*>& to_visit) {
	if (visited == Visit::First) {
		cout << "![";
		to_visit.push(this);
		to_visit.push(left_child);
		visited = Visit::Second;
	} else if (visited == Visit::Second) {
		cout << ']';
		visited = Visit::First;
	}
}

void Formula::printVar() {
	cout << name;
}

void Formula::printSymbol() {
	switch (op) {
		case Op::Conj:
			cout << ")&(";
			break;
		case Op::Dis:
			cout << ")|(";
			break;
		case Op::Edis:
			cout << ")^(";
			break;
		case Op::Mcond:
			cout << ")>(";
			break;
		case Op::Leq:
			cout << ")=(";
			break;
		default:
			throw(InvalidStringException());
			break;
	}
}

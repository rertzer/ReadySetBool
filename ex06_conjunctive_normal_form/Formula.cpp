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
	if (left_child != nullptr) {
		left_child = new Formula(*left_child);
	}
	if (right_child != nullptr) {
		right_child = new Formula(*right_child);
	}
}

Formula::Formula(Formula&& f) {
	*this = f;
	f.parent = nullptr;
	f.left_child = nullptr;
	f.right_child = nullptr;
}

Formula::~Formula() {
	if (left_child != nullptr) {
		delete left_child;
	}
	if (right_child != nullptr) {
		delete right_child;
	}
}

Formula& Formula::operator=(Formula const& f) {
	if (this != &f) {
		kind = f.kind;
		op = f.op;
		name = f.name;
		visited = f.visited;
		parent = f.parent;
		if (left_child != nullptr) {
			delete left_child;
		}
		left_child = f.left_child;
		if (right_child != nullptr) {
			delete right_child;
		}
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

void Formula::kill() {
	left_child = nullptr;
	right_child = nullptr;
	delete this;
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
void Formula::cnf() {
	rewrite();
}

void Formula::rewrite() {
	SuperStack<Formula*> to_visit;
	to_visit.push(this);
	while (!to_visit.empty()) {
		// cout << "rewrite\n";
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
	// cout << "rewrite child\n";
	// print();
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
				child->rewriteMaterialCondition();
				to_visit.push(this);
				break;
			case Op::Leq:
				child->rewriteEquivalence();
				to_visit.push(this);
				break;
			case Op::Edis:
				child->rewriteExclusiveDisjunction();
				to_visit.push(this);
				break;
			case Op::Dis:
				child->rewriteDisjunction(to_visit);
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
	child->kill();
	kid->op = Op::Dis;
	kid->left_child = kid->left_child->negate();
	kid->right_child = kid->right_child->negate();
	to_visit.push(this);
	// cout << "rmc\n";
	return (kid);
}

Formula* Formula::rewriteMorganDis(SuperStack<Formula*>& to_visit, Formula* child) {
	Formula* kid = child->left_child;
	child->kill();
	kid->op = Op::Conj;
	kid->left_child = kid->left_child->negate();
	kid->right_child = kid->right_child->negate();
	to_visit.push(this);

	// cout << "rmd\n";
	return (kid);
}

Formula* Formula::rewriteDoubleNegation(SuperStack<Formula*>& to_visit, Formula* child) {
	Formula* kid = child->left_child->left_child;
	child->left_child->kill();
	child->kill();
	to_visit.push(this);
	// cout << "rdn\n";
	return (kid);
}

void Formula::rewriteMaterialCondition() {
	left_child = left_child->negate();
	op = Op::Dis;

	// cout << "rmc\n";
}

void Formula::rewriteEquivalence() {
	Formula* left_kid = left_child;
	Formula* right_kid = right_child;

	left_child = new Formula('>');
	right_child = new Formula('>');

	left_child->left_child = left_kid;
	left_child->right_child = right_kid;
	right_child->left_child = new Formula(*right_kid);
	right_child->right_child = new Formula(*left_kid);

	// cout << "req\n";
	op = Op::Conj;
}

void Formula::rewriteExclusiveDisjunction() {
	Formula* left_kid = left_child;
	Formula* right_kid = right_child;

	left_child = new Formula('|');
	right_child = new Formula('|');

	right_child->right_child = new Formula(*right_kid);
	right_child->left_child = new Formula(*left_kid);
	left_child->left_child = left_kid->negate();
	left_child->right_child = right_kid->negate();

	// cout << "red\n";
	op = Op::Conj;
}

void Formula::rewriteDisjunction(SuperStack<Formula*>& to_visit) {
	// cout << "rdis\n";
	if (right_child->op == Op::Conj) {
		rewriteDisjunctionRight();
		to_visit.push(this);
	} else if (left_child->op == Op::Conj) {
		rewriteDisjunctionLeft();
		to_visit.push(this);
	}
}

void Formula::rewriteDisjunctionLeft() {
	Formula* right_kid = right_child;

	right_child = new Formula('|');
	right_child->left_child = left_child->right_child;
	right_child->right_child = right_kid;
	left_child->op = Op::Dis;
	left_child->right_child = new Formula(*right_kid);
	op = Op::Conj;
	// cout << "rdisL\n";
}

void Formula::rewriteDisjunctionRight() {
	Formula* left_kid = left_child;

	left_child = new Formula('|');
	left_child->left_child = left_kid;
	left_child->right_child = right_child->left_child;
	right_child->op = Op::Dis;
	right_child->left_child = new Formula(*left_kid);
	op = Op::Conj;
	// cout << "rdisR\n";
}

Formula* Formula::negate() {
	Formula* neg = new Formula('!');
	neg->left_child = this;

	return (neg);
}

string Formula::revertPolish() {
	SuperStack<Formula*> to_visit;
	string				 rp;
	string				 ops;
	to_visit.push(this);
	while (!to_visit.empty()) {
		revertNode(rp, ops, to_visit);
	}

	return (rp);
}

void Formula::revertNode(string& rp, string& ops, SuperStack<Formula*>& to_visit) {
	Formula* current_node = to_visit.popout();

	switch (current_node->kind) {
		case Kind::Root:
			current_node->revertRoot(rp, ops, to_visit);
			break;
		case Kind::Op:
			current_node->revertOp(rp, ops, to_visit);
			break;
		case Kind::Neg:
			current_node->revertNeg(rp, to_visit);
			break;
		case Kind::Var:
			current_node->revertVar(rp);
			break;
	}
	// cout << rp << " " << ops << endl;
}

void Formula::revertRoot(string& rp, string& ops, SuperStack<Formula*>& to_reverse) {
	if (visited == Visit::First) {
		to_reverse.push(this);
		to_reverse.push(left_child);
		visited = Visit::Second;
	} else {
		// reverse(rp.begin(), rp.end());
		if (!ops.empty()) {
			reverse(ops.begin(), ops.end());
			rp.append(ops);
		}
		visited = Visit::First;
	}
}

void Formula::revertOp(string& rp, string& ops, SuperStack<Formula*>& to_reverse) {
	char s = getSymbol();
	if (visited == Visit::First) {
		ops.push_back(s);
		to_reverse.push(right_child);
		to_reverse.push(this);
		to_reverse.push(left_child);
		visited = Visit::Second;
	} else {
		// string tmp;
		while ((!ops.empty()) && (ops.back() != s)) {
			rp.push_back(ops.back());
			ops.pop_back();
		}
		// rp.insert(0, tmp);
		visited = Visit::First;
	}
}

void Formula::revertNeg(string& rp, SuperStack<Formula*>& to_reverse) {
	if (visited == Visit::First) {
		to_reverse.push(this);
		to_reverse.push(left_child);
		visited = Visit::Second;
	} else {
		char s = getSymbol();
		rp.push_back(s);
		visited = Visit::First;
	}
}

void Formula::revertVar(string& rp) {
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

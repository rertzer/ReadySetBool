#include "Formula.hpp"
#include <memory>
#include "SuperStack.hpp"

Formula::Formula()
	: kind(Kind::Root),
	  op(Op::Na),
	  name('\0'),
	  visited(Visit::First),
	  left_child(nullptr),
	  right_child(nullptr) {}

Formula::Formula(char s)
	: kind(charToKind(s)),
	  op(charToOp(s)),
	  name(charToName(s)),
	  visited(Visit::First),
	  left_child(nullptr),
	  right_child(nullptr) {}

Formula::Formula(shared_ptr<Formula> f)
	: kind(f->kind),
	  op(f->op),
	  name(f->name),
	  visited(f->visited),
	  left_child(nullptr),
	  right_child(nullptr) {
	if (f->left_child != nullptr) {
		left_child = make_shared<Formula>(f->left_child);
	}
	if (f->right_child != nullptr) {
		right_child = make_shared<Formula>(f->right_child);
	}
}

Formula::Formula(Formula const& f)
	: kind(f.kind),
	  op(f.op),
	  name(f.name),
	  visited(f.visited),
	  left_child(nullptr),
	  right_child(nullptr) {
	if (f.left_child != nullptr) {
		left_child = make_shared<Formula>(f.left_child);
	}
	if (f.right_child != nullptr) {
		right_child = make_shared<Formula>(f.right_child);
	}
}

Formula::Formula(Formula&& f) : left_child(nullptr), right_child(nullptr) {
	*shared_from_this() = f;
	f.left_child.reset();
	f.right_child.reset();
}

Formula::~Formula() {
	if (left_child != nullptr) {
		left_child.reset();
	}
	if (right_child != nullptr) {
		right_child.reset();
	}
}

Formula& Formula::operator=(Formula const& f) {
	if (shared_from_this().get() != &f) {
		kind = f.kind;
		op = f.op;
		name = f.name;
		visited = f.visited;
		if (left_child != nullptr) {
			left_child.reset();
		}
		left_child = f.left_child;
		if (right_child != nullptr) {
			right_child.reset();
		}
		right_child = f.right_child;
	}
	return (*shared_from_this());
}

Formula& Formula::operator=(Formula&& f) {
	*shared_from_this() = f;
	f.left_child.reset();
	f.right_child.reset();
	return (*shared_from_this());
}

void Formula::kill() {
	left_child.reset();
	right_child.reset();
	shared_from_this().reset();
}

void Formula::erase() {
	if (left_child != nullptr) {
		left_child->erase();
	}
	if (right_child != nullptr) {
		right_child->erase();
	}
	if (kind != Kind::Root)
		shared_from_this()->~Formula();
}
void Formula::cnf() {
	rewrite();
}

void Formula::rewrite() {
	SuperStack<weak_ptr<Formula>> to_visit;
	to_visit.push(shared_from_this());
	while (!to_visit.empty()) {
		rewriteNode(to_visit);
	}
}

void Formula::rewriteNode(SuperStack<weak_ptr<Formula>>& to_visit) {
	weak_ptr<Formula> weak_node = to_visit.popout();

	if (weak_node.expired()) {
		return;
	}

	shared_ptr<Formula> current_node = weak_node.lock();

	if (current_node->left_child != nullptr) {
		current_node->left_child = rewriteChild(to_visit, current_node->left_child);
	}
	if (current_node->right_child != nullptr) {
		current_node->right_child = rewriteChild(to_visit, current_node->right_child);
	}
}

shared_ptr<Formula> Formula::rewriteChild(SuperStack<weak_ptr<Formula>>& to_visit,
										  shared_ptr<Formula>			 child) {
	shared_ptr<Formula> kid = child;

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
				to_visit.push(shared_from_this());
				break;
			case Op::Leq:
				child->rewriteEquivalence();
				to_visit.push(shared_from_this());
				break;
			case Op::Edis:
				child->rewriteExclusiveDisjunction();
				to_visit.push(shared_from_this());
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

shared_ptr<Formula> Formula::rewriteMorganConj(SuperStack<weak_ptr<Formula>>& to_visit,
											   shared_ptr<Formula>			  child) {
	shared_ptr<Formula> kid = child->left_child;

	child->kill();
	kid->op = Op::Dis;
	kid->left_child = kid->left_child->negate();
	kid->right_child = kid->right_child->negate();
	to_visit.push(shared_from_this());

	return (kid);
}

shared_ptr<Formula> Formula::rewriteMorganDis(SuperStack<weak_ptr<Formula>>& to_visit,
											  shared_ptr<Formula>			 child) {
	shared_ptr<Formula> kid = child->left_child;

	child->kill();
	kid->op = Op::Conj;
	kid->left_child = kid->left_child->negate();
	kid->right_child = kid->right_child->negate();
	to_visit.push(shared_from_this());

	return (kid);
}

shared_ptr<Formula> Formula::rewriteDoubleNegation(SuperStack<weak_ptr<Formula>>& to_visit,
												   shared_ptr<Formula>			  child) {
	shared_ptr<Formula> kid = child->left_child->left_child;

	child->left_child->kill();
	child->kill();
	to_visit.push(shared_from_this());

	return (kid);
}

void Formula::rewriteMaterialCondition() {
	left_child = left_child->negate();
	op = Op::Dis;
}

void Formula::rewriteEquivalence() {
	shared_ptr<Formula> left_kid = left_child;
	shared_ptr<Formula> right_kid = right_child;

	left_child = make_shared<Formula>('>');
	right_child = make_shared<Formula>('>');

	left_child->left_child = left_kid;
	left_child->right_child = right_kid;
	right_child->left_child = make_shared<Formula>(right_kid);
	right_child->right_child = make_shared<Formula>(left_kid);

	op = Op::Conj;
}

void Formula::rewriteExclusiveDisjunction() {
	shared_ptr<Formula> left_kid = left_child;
	shared_ptr<Formula> right_kid = right_child;

	left_child = make_shared<Formula>('|');
	right_child = make_shared<Formula>('|');

	right_child->right_child = make_shared<Formula>(right_kid);
	right_child->left_child = make_shared<Formula>(left_kid);
	left_child->left_child = left_kid->negate();
	left_child->right_child = right_kid->negate();

	op = Op::Conj;
}

void Formula::rewriteDisjunction(SuperStack<weak_ptr<Formula>>& to_visit) {
	if (right_child->op == Op::Conj) {
		rewriteDisjunctionRight();
		to_visit.push(shared_from_this());
	} else if (left_child->op == Op::Conj) {
		rewriteDisjunctionLeft();
		to_visit.push(shared_from_this());
	}
}

void Formula::rewriteDisjunctionLeft() {
	shared_ptr<Formula> right_kid = right_child;

	right_child = make_shared<Formula>('|');
	right_child->left_child = left_child->right_child;
	right_child->right_child = right_kid;
	left_child->op = Op::Dis;
	left_child->right_child = make_shared<Formula>(*right_kid);
	op = Op::Conj;
}

void Formula::rewriteDisjunctionRight() {
	shared_ptr<Formula> left_kid = left_child;

	left_child = make_shared<Formula>('|');
	left_child->left_child = left_kid;
	left_child->right_child = right_child->left_child;
	right_child->op = Op::Dis;
	right_child->left_child = make_shared<Formula>(left_kid);
	op = Op::Conj;
}

shared_ptr<Formula> Formula::negate() {
	shared_ptr<Formula> neg = make_shared<Formula>('!');

	neg->left_child = shared_from_this();

	return (neg);
}

string Formula::revertPolish() {
	SuperStack<shared_ptr<Formula>> to_visit;
	string							rp;
	string							ops;

	to_visit.push(shared_from_this());
	while (!to_visit.empty()) {
		revertNode(rp, ops, to_visit);
	}

	return (rp);
}

void Formula::revertNode(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_visit) {
	shared_ptr<Formula> current_node = to_visit.popout();

	switch (current_node->kind) {
		case Kind::Root:
			current_node->revertRoot(rp, ops, to_visit);
			break;
		case Kind::Op:
			current_node->revertOp(rp, ops, to_visit);
			break;
		case Kind::Neg:
			current_node->revertNeg(rp, ops, to_visit);
			break;
		case Kind::Var:
			current_node->revertVar(rp);
			break;
	}
}

void Formula::revertRoot(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_reverse) {
	if (visited == Visit::First) {
		to_reverse.push(shared_from_this());
		to_reverse.push(left_child);
		visited = Visit::Second;
	} else {
		if (!ops.empty()) {
			reverse(ops.begin(), ops.end());
			rp.append(ops);
		}
		visited = Visit::First;
	}
}

void Formula::revertOp(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_reverse) {
	char s = getSymbol();
	if (visited == Visit::First) {
		ops.push_back(s);
		to_reverse.push(right_child);
		to_reverse.push(shared_from_this());
		to_reverse.push(left_child);
		visited = Visit::Second;
	} else {
		while ((!ops.empty()) && (ops.back() != s)) {
			rp.push_back(ops.back());
			ops.pop_back();
		}
		visited = Visit::First;
	}
}

void Formula::revertNeg(string& rp, string& ops, SuperStack<shared_ptr<Formula>>& to_reverse) {
	if (visited == Visit::First) {
		to_reverse.push(shared_from_this());
		to_reverse.push(left_child);
		visited = Visit::Second;
	} else {
		if (shared_from_this()->left_child->kind == Kind::Op) {
			char cs = shared_from_this()->left_child->getSymbol();
			while ((!ops.empty()) && (ops.back() != cs)) {
				rp.push_back(ops.back());
				ops.pop_back();
			}
			if ((!ops.empty()) && (ops.back() == cs)) {
				rp.push_back(ops.back());
				ops.pop_back();
			}
		}
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
			s = '\\';
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

void Formula::fromString(string rp) {
	SuperStack<shared_ptr<Formula>> to_visit;
	to_visit.push(shared_from_this());
	while (!rp.empty()) {
		addChildsFromString(rp, to_visit);
	}
	if (!to_visit.empty())
		throw(InvalidStringException());
}

void Formula::addChildsFromString(string& rp, SuperStack<shared_ptr<Formula>>& to_visit) {
	try {
		shared_ptr<Formula> current_node = to_visit.popout();

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
	} catch (SuperStack<shared_ptr<Formula>>::SuperStackEmptyException& e) {
		cout << e.what() << "\n";
		shared_from_this()->~Formula();
		throw(InvalidStringException());
	}
}

void Formula::addChildToRoot(string& rp, SuperStack<shared_ptr<Formula>>& to_visit) {
	char s = extractNextSymbol(rp);
	left_child = make_shared<Formula>(s);
	if (left_child->kind != Kind::Var)
		to_visit.push(left_child);
}

void Formula::addChildsToOp(string& rp, SuperStack<shared_ptr<Formula>>& to_visit) {
	if (visited == Visit::First) {
		char s = extractNextSymbol(rp);
		right_child = make_shared<Formula>(s);
		to_visit.push(shared_from_this());
		if (right_child->kind != Kind::Var)
			to_visit.push(right_child);
		visited = Visit::Second;
	} else if (visited == Visit::Second) {
		char s = extractNextSymbol(rp);
		left_child = make_shared<Formula>(s);
		if (left_child->kind != Kind::Var)
			to_visit.push(left_child);
		visited = Visit::First;
	}
}

void Formula::addChildToNeg(string& rp, SuperStack<shared_ptr<Formula>>& to_visit) {
	char s = extractNextSymbol(rp);
	left_child = make_shared<Formula>(s);
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
	SuperStack<shared_ptr<Formula>> to_visit;
	to_visit.push(shared_from_this());

	while (!to_visit.empty()) {
		shared_ptr<Formula> node = to_visit.popout();
		node->printNode(to_visit);
	}
	cout << endl;
}

void Formula::printNode(SuperStack<shared_ptr<Formula>>& to_visit) {
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

void Formula::printRoot(SuperStack<shared_ptr<Formula>>& to_visit) {
	to_visit.push(left_child);
}

void Formula::printOp(SuperStack<shared_ptr<Formula>>& to_visit) {
	if (visited == Visit::First) {
		cout << '(';
		to_visit.push(shared_from_this());
		to_visit.push(right_child);
		to_visit.push(shared_from_this());
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

void Formula::printNeg(SuperStack<shared_ptr<Formula>>& to_visit) {
	if (visited == Visit::First) {
		cout << "!" << "[";
		to_visit.push(shared_from_this());
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

#include "Formula.hpp"

Formula::Formula()
	: kind(Kind::Root),
	  op(Op::Na),
	  name(""),
	  parent(nullptr),
	  left_child(nullptr),
	  right_child(nullptr) {}

Formula::Formula(string rp)
	: kind(Kind::Root),
	  op(Op::Na),
	  name(""),
	  parent(nullptr),
	  left_child(nullptr),
	  right_child(nullptr) {
	fromString(rp);
}

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
		return;
	addChildsFromString(rp);
}

void Formula::addChildsFromString(string rp) {
	Kind next_kind = extractKind(rp);

	switch (next_kind) {
		case Kind::Op:
			addOp(rp);
			break;
		case Kind::Neg:
			addNeg(rp);
			break;
	}
}

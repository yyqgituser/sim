#include "Declarator.h"

#include "NodeVisitor.h"

namespace node {

Declarator::Declarator(shared_ptr<Term> name, shared_ptr<Node> init) :
		name(name), init(init) {
}

Declarator::~Declarator() {
}

void Declarator::accept(NodeVisitor *visitor) {
	visitor->visitDeclarator(shared_from_this());
}

Location Declarator::getLocation() {
	return name->getLocation();
}

} /* namespace node */

#include "Identifier.h"

#include "NodeVisitor.h"

namespace node {

Identifier::Identifier(shared_ptr<Term> name) :
		name(name) {
}

Identifier::~Identifier() {
}

void Identifier::accept(NodeVisitor *visitor) {
	visitor->visitIdentifier(shared_from_this());
}

Location Identifier::getLocation() {
	return name->getLocation();
}

} /* namespace node */

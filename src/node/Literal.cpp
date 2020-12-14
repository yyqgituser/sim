#include "Literal.h"

#include "NodeVisitor.h"

namespace node {

Literal::Literal(shared_ptr<Term> term) :
		term(term) {
}

Literal::~Literal() {
}

void Literal::accept(NodeVisitor *visitor) {
	visitor->visitLiteral(shared_from_this());
}

Location Literal::getLocation() {
	return term->getLocation();
}

} /* namespace node */

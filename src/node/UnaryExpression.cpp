#include "UnaryExpression.h"

#include "NodeVisitor.h"

namespace node {

UnaryExpression::UnaryExpression(shared_ptr<Term> op, shared_ptr<Node> exp) :
		op(op), exp(exp) {
}

UnaryExpression::~UnaryExpression() {
}

void UnaryExpression::accept(NodeVisitor *visitor) {
	visitor->visitUnaryExpression(shared_from_this());
}

Location UnaryExpression::getLocation() {
	return op->getLocation();
}

} /* namespace node */

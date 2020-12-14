#include "BinaryExpression.h"

#include "NodeVisitor.h"

namespace node {

BinaryExpression::BinaryExpression(shared_ptr<Term> op, shared_ptr<Node> left,
		shared_ptr<Node> right) :
		op(op), left(left), right(right) {
}

BinaryExpression::~BinaryExpression() {
}

void BinaryExpression::accept(NodeVisitor *visitor) {
	visitor->visitBinaryExpression(shared_from_this());
}

Location BinaryExpression::getLocation() {
	return op->getLocation();
}

} /* namespace node */

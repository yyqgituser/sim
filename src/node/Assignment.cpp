#include "Assignment.h"

#include "NodeVisitor.h"

namespace node {

Assignment::Assignment(shared_ptr<Node> left, shared_ptr<Node> right, shared_ptr<Term> op) :
		left(left), right(right), op(op) {
}

Assignment::~Assignment() {
}

void Assignment::accept(NodeVisitor *visitor) {
	visitor->visitAssignment(shared_from_this());
}

Location Assignment::getLocation() {
	return op->getLocation();
}

} /* namespace node */

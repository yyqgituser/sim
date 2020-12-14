#include "XcreaseStatement.h"

#include "NodeVisitor.h"

namespace node {

XcreaseStatement::XcreaseStatement(shared_ptr<Node> exp, shared_ptr<Term> op): exp(exp), op(op) {
}

XcreaseStatement::~XcreaseStatement() {
}

void XcreaseStatement::accept(NodeVisitor *visitor) {
	visitor->visitXcreaseStatement(shared_from_this());
}

Location XcreaseStatement::getLocation() {
	return op->getLocation();
}

} /* namespace node */

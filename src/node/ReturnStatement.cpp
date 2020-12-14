#include "ReturnStatement.h"

#include "NodeVisitor.h"

namespace node {

ReturnStatement::ReturnStatement(Location loc) :
		ReturnStatement(0, loc) {
}

ReturnStatement::ReturnStatement(shared_ptr<Node> exp, Location loc) :
		exp(exp), loc(loc) {
}

ReturnStatement::~ReturnStatement() {
}

void ReturnStatement::accept(NodeVisitor *visitor) {
	visitor->visitReturnStatement(shared_from_this());
}

Location ReturnStatement::getLocation() {
	return loc;
}

} /* namespace node */

#include "WhileStatement.h"

#include "NodeVisitor.h"

namespace node {

WhileStatement::WhileStatement(shared_ptr<Node> condExp, shared_ptr<Node> body, Location loc) :
		condExp(condExp), body(body), loc(loc) {
}

WhileStatement::~WhileStatement() {
}

void WhileStatement::accept(NodeVisitor *visitor) {
	visitor->visitWhileStatement(shared_from_this());
}

Location WhileStatement::getLocation() {
	return loc;
}

} /* namespace node */

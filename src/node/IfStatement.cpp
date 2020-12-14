#include "IfStatement.h"

#include "NodeVisitor.h"

namespace node {

IfStatement::IfStatement(shared_ptr<Node> condExp, shared_ptr<Node> thenStmt, shared_ptr<Node> elseStmt,
		Location loc) :
		condExp(condExp), thenStmt(thenStmt), elseStmt(elseStmt), loc(loc) {
}

IfStatement::~IfStatement() {
}

void IfStatement::accept(NodeVisitor *visitor) {
	visitor->visitIfStatement(shared_from_this());
}

Location IfStatement::getLocation() {
	return loc;
}

} /* namespace node */

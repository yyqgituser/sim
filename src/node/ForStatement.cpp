#include "ForStatement.h"

#include "NodeVisitor.h"

namespace node {

ForStatement::ForStatement(shared_ptr<Node> init, shared_ptr<Node> condExp, shared_ptr<Node> update,
		shared_ptr<Node> body, Location loc) :
		init(init), condExp(condExp), update(update), body(body), loc(loc) {
}

ForStatement::~ForStatement() {
}

void ForStatement::accept(NodeVisitor *visitor) {
	visitor->visitForStatement(shared_from_this());
}

Location ForStatement::getLocation() {
	return loc;
}

} /* namespace node */

#include "ContinueStatement.h"

#include "NodeVisitor.h"

namespace node {

ContinueStatement::ContinueStatement(Location loc) :
		loc(loc) {
}

ContinueStatement::~ContinueStatement() {
}

void ContinueStatement::accept(NodeVisitor *visitor) {
	visitor->visitContinueStatement(shared_from_this());
}

Location ContinueStatement::getLocation() {
	return loc;
}

} /* namespace node */

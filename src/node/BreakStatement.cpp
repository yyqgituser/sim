#include "BreakStatement.h"

#include "NodeVisitor.h"

namespace node {

BreakStatement::BreakStatement(Location loc) :
		loc(loc) {
}

BreakStatement::~BreakStatement() {
}

void BreakStatement::accept(NodeVisitor *visitor) {
	visitor->visitBreakStatement(shared_from_this());
}

Location BreakStatement::getLocation() {
	return loc;
}

} /* namespace node */

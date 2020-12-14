#include "EmptyStatement.h"

#include "NodeVisitor.h"

namespace node {

EmptyStatement::EmptyStatement(Location loc) :
		loc(loc) {
}

EmptyStatement::~EmptyStatement() {
}

void EmptyStatement::accept(NodeVisitor *visitor) {
	visitor->visitEmptyStatement(shared_from_this());
}

Location EmptyStatement::getLocation() {
	return loc;
}

} /* namespace node */

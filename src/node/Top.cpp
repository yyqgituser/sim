#include "Top.h"

#include "NodeVisitor.h"

namespace node {

Top::Top(shared_ptr<vector<shared_ptr<Node>>> members) :
		members(members) {
}

Top::~Top() {
}

void Top::accept(NodeVisitor *visitor) {
	visitor->visitTop(shared_from_this());
}

Location Top::getLocation() {
	return Location(1, 1);
}

} /* namespace node */

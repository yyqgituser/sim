#include "ArrayInitializer.h"

#include "NodeVisitor.h"

namespace node {

ArrayInitializer::ArrayInitializer(shared_ptr<vector<shared_ptr<Node>>> inits, Location loc) :
		inits(inits), loc(loc) {
}

ArrayInitializer::~ArrayInitializer() {
}

void ArrayInitializer::accept(NodeVisitor *visitor) {
	visitor->visitArrayInitializer(shared_from_this());
}

Location ArrayInitializer::getLocation() {
	return loc;
}

} /* namespace node */

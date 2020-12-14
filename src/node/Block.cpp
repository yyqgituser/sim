#include "Block.h"

#include "NodeVisitor.h"

namespace node {

Block::Block(shared_ptr<vector<shared_ptr<Node>>> elems, Location loc) :
		elems(elems), loc(loc) {
}

Block::~Block() {
}

void Block::accept(NodeVisitor *visitor) {
	visitor->visitBlock(shared_from_this());
}

Location Block::getLocation() {
	return loc;
}

} /* namespace node */

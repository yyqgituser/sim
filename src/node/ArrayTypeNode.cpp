#include "ArrayTypeNode.h"

#include "NodeVisitor.h"

namespace node {

ArrayTypeNode::ArrayTypeNode(shared_ptr<Node> typeNode) :
		typeNode(typeNode) {
}

ArrayTypeNode::~ArrayTypeNode() {
}

void ArrayTypeNode::accept(NodeVisitor *visitor) {
	visitor->visitArrayTypeNode(shared_from_this());
}

Location ArrayTypeNode::getLocation() {
	return typeNode->getLocation();
}

} /* namespace node */

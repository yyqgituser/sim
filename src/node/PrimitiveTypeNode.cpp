#include "PrimitiveTypeNode.h"

#include "NodeVisitor.h"

namespace node {

PrimitiveTypeNode::PrimitiveTypeNode(shared_ptr<Term> term) :
		term(term) {
}

PrimitiveTypeNode::~PrimitiveTypeNode() {
}

void PrimitiveTypeNode::accept(NodeVisitor *visitor) {
	visitor->visitPrimitiveTypeNode(shared_from_this());
}

Location PrimitiveTypeNode::getLocation() {
	return term->getLocation();
}

} /* namespace node */

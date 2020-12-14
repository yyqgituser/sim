#include "CastExpression.h"

#include "NodeVisitor.h"

namespace node {

CastExpression::CastExpression(shared_ptr<Node> typeNode, shared_ptr<Node> exp) :
		typeNode(typeNode), exp(exp) {

}

CastExpression::~CastExpression() {
}

void CastExpression::accept(NodeVisitor *visitor) {
	visitor->visitCastExpression(shared_from_this());
}

Location CastExpression::getLocation() {
	return typeNode->getLocation();
}

} /* namespace node */

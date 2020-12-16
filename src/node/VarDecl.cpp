#include "VarDecl.h"

#include "NodeVisitor.h"

namespace node {

VarDecl::VarDecl(shared_ptr<Node> typeNode, shared_ptr<vector<shared_ptr<Node>>> decls) :
		typeNode(typeNode), decls(decls) {
}

VarDecl::~VarDecl() {
}

void VarDecl::accept(NodeVisitor *visitor) {
	visitor->visitVarDecl(shared_from_this());
}

Location VarDecl::getLocation() {
	return typeNode->getLocation();
}

} /* namespace node */

#include "VarDecl.h"

#include "NodeVisitor.h"

namespace node {

VarDecl::VarDecl(shared_ptr<Node> typeNode, shared_ptr<Term> name, shared_ptr<Node> init) :
		typeNode(typeNode), name(name), init(init) {
}

VarDecl::~VarDecl() {
}

void VarDecl::accept(NodeVisitor *visitor) {
	visitor->visitVarDecl(shared_from_this());
}

Location VarDecl::getLocation() {
	return name->getLocation();
}

} /* namespace node */

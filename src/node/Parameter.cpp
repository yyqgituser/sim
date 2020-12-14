#include "Parameter.h"

#include "NodeVisitor.h"

namespace node {

Parameter::Parameter(shared_ptr<Node> typeNode, shared_ptr<Term> name) :
		typeNode(typeNode), name(name) {
}

Parameter::~Parameter() {
}

void Parameter::accept(NodeVisitor *visitor) {
	visitor->visitParameter(shared_from_this());
}

Location Parameter::getLocation() {
	return name->getLocation();
}

} /* namespace node */

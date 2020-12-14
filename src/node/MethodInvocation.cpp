#include "MethodInvocation.h"

#include "NodeVisitor.h"

namespace node {

MethodInvocation::MethodInvocation(shared_ptr<Term> name, shared_ptr<vector<shared_ptr<Node>>> args) :
		name(name), args(args) {
}

MethodInvocation::~MethodInvocation() {
}

void MethodInvocation::accept(NodeVisitor *visitor) {
	visitor->visitMethodInvocation(shared_from_this());
}

Location MethodInvocation::getLocation() {
	return name->getLocation();
}

} /* namespace node */

#include "Method.h"

#include "NodeVisitor.h"

namespace node {

Method::Method(shared_ptr<Node> returnType, shared_ptr<Term> name,
		shared_ptr<vector<shared_ptr<Node>>> params, shared_ptr<Node> body, bool isVarParam) :
		returnType(returnType), name(name), params(params), body(body), isVarParam(isVarParam) {
}

Method::~Method() {
}

void Method::accept(NodeVisitor *visitor) {
	visitor->visitMethod(shared_from_this());
}

Location Method::getLocation() {
	return name->getLocation();
}

} /* namespace node */

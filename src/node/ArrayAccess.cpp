#include "ArrayAccess.h"

#include "NodeVisitor.h"

namespace node {

ArrayAccess::ArrayAccess(shared_ptr<Node> array, shared_ptr<Node> index) :
		array(array), index(index) {
}

ArrayAccess::~ArrayAccess() {
}

void ArrayAccess::accept(NodeVisitor *visitor) {
	visitor->visitArrayAccess(shared_from_this());
}

Location ArrayAccess::getLocation() {
	return array->getLocation();
}

} /* namespace node */

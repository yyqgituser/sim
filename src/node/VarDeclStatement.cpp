#include "VarDeclStatement.h"

#include "NodeVisitor.h"

namespace node {

VarDeclStatement::VarDeclStatement(shared_ptr<Node> varDecl) :
		varDecl(varDecl) {
}

VarDeclStatement::~VarDeclStatement() {
}

void VarDeclStatement::accept(NodeVisitor *visitor) {
	visitor->visitVarDeclStatement(shared_from_this());
}

Location VarDeclStatement::getLocation() {
	return varDecl->getLocation();
}

} /* namespace node */

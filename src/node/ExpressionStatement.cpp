#include "ExpressionStatement.h"

#include "NodeVisitor.h"

namespace node {

ExpressionStatement::ExpressionStatement(shared_ptr<Node> exp) :
		exp(exp) {
}

ExpressionStatement::~ExpressionStatement() {
}

void ExpressionStatement::accept(NodeVisitor *visitor) {
	visitor->visitExpressionStatement(shared_from_this());
}

Location ExpressionStatement::getLocation() {
	return exp->getLocation();
}

}
/* namespace node */

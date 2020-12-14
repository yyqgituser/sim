#include "Term.h"

#include "NodeVisitor.h"

namespace node {

Term::Term(SimToken *token) :
		token(token) {
}

Term::~Term() {
	delete token;
}

void Term::accept(NodeVisitor *visitor) {
	visitor->visitTerm(shared_from_this());
}

Location Term::getLocation() {
	return Location(token->line, token->column);
}

}

#ifndef SRC_NODE_UNARYEXPRESSION_H_
#define SRC_NODE_UNARYEXPRESSION_H_

#include <memory>
#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class UnaryExpression: public Node, public enable_shared_from_this<UnaryExpression> {
public:
	shared_ptr<Term> op;

	shared_ptr<Node> exp;

	UnaryExpression(shared_ptr<Term> op, shared_ptr<Node> exp);
	virtual ~UnaryExpression();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_UNARYEXPRESSION_H_ */

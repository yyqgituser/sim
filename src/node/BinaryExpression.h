#ifndef SRC_NODE_BINARYEXPRESSION_H_
#define SRC_NODE_BINARYEXPRESSION_H_

#include <memory>
#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class BinaryExpression: public Node, public enable_shared_from_this<BinaryExpression> {
public:
	shared_ptr<Term> op;

	shared_ptr<Node> left;

	shared_ptr<Node> right;

	BinaryExpression(shared_ptr<Term> op, shared_ptr<Node> left, shared_ptr<Node> right);
	virtual ~BinaryExpression();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_BINARYEXPRESSION_H_ */

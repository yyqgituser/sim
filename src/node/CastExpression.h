#ifndef SRC_NODE_CASTEXPRESSION_H_
#define SRC_NODE_CASTEXPRESSION_H_

#include <memory>
#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class CastExpression: public Node, public enable_shared_from_this<CastExpression> {
public:
	shared_ptr<Node> typeNode;

	shared_ptr<Node> exp;

	CastExpression(shared_ptr<Node> typeNode, shared_ptr<Node> exp);
	virtual ~CastExpression();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_CASTEXPRESSION_H_ */

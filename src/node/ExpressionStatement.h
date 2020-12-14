#ifndef SRC_NODE_EXPRESSIONSTATEMENT_H_
#define SRC_NODE_EXPRESSIONSTATEMENT_H_

#include <memory>

#include "Node.h"

using namespace std;

namespace node {

class ExpressionStatement: public Node, public enable_shared_from_this<ExpressionStatement> {
public:
	shared_ptr<Node> exp;

	ExpressionStatement(shared_ptr<Node> exp);
	virtual ~ExpressionStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_EXPRESSIONSTATEMENT_H_ */

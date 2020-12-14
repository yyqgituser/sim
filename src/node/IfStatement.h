#ifndef SRC_NODE_IFSTATEMENT_H_
#define SRC_NODE_IFSTATEMENT_H_
#include <memory>
#include "Node.h"

using namespace std;

namespace node {

class IfStatement: public Node, public enable_shared_from_this<IfStatement> {
public:
	shared_ptr<Node> condExp;

	shared_ptr<Node> thenStmt;

	shared_ptr<Node> elseStmt;

	Location loc;

	IfStatement(shared_ptr<Node> condExp, shared_ptr<Node> thenStmt, shared_ptr<Node> elseStmt, Location loc);
	virtual ~IfStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_IFSTATEMENT_H_ */

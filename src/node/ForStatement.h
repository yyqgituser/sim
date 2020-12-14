#ifndef SRC_NODE_FORSTATEMENT_H_
#define SRC_NODE_FORSTATEMENT_H_

#include <memory>
#include "Node.h"

using namespace std;

namespace node {

class ForStatement: public Node, public enable_shared_from_this<ForStatement> {
public:
	shared_ptr<Node> init;

	shared_ptr<Node> condExp;

	shared_ptr<Node> update;

	shared_ptr<Node> body;

	Location loc;

	ForStatement(shared_ptr<Node> init, shared_ptr<Node> condExp, shared_ptr<Node> update,
			shared_ptr<Node> body, Location loc);
	virtual ~ForStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_FORSTATEMENT_H_ */

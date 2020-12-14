#ifndef SRC_NODE_WHILESTATEMENT_H_
#define SRC_NODE_WHILESTATEMENT_H_

#include <memory>
#include "Node.h"

using namespace std;

namespace node {

class WhileStatement: public Node, public enable_shared_from_this<WhileStatement> {
public:
	shared_ptr<Node> condExp;

	shared_ptr<Node> body;

	Location loc;

	WhileStatement(shared_ptr<Node> condExp, shared_ptr<Node> body, Location loc);
	virtual ~WhileStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_WHILESTATEMENT_H_ */

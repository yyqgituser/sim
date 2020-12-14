#ifndef SRC_NODE_RETURNSTATEMENT_H_
#define SRC_NODE_RETURNSTATEMENT_H_

#include <memory>
#include "Node.h"

using namespace std;

namespace node {

class ReturnStatement: public Node, public enable_shared_from_this<ReturnStatement> {
public:
	shared_ptr<Node> exp;

	Location loc;

	ReturnStatement(Location loc);
	ReturnStatement(shared_ptr<Node> exp, Location loc);
	virtual ~ReturnStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_CONTINUESTATEMENT_H_ */

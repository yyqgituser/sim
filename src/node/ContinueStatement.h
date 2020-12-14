#ifndef SRC_NODE_CONTINUESTATEMENT_H_
#define SRC_NODE_CONTINUESTATEMENT_H_

#include <memory>
#include "Node.h"

using namespace std;

namespace node {

class ContinueStatement: public Node, public enable_shared_from_this<ContinueStatement> {
public:
	Location loc;

	ContinueStatement(Location loc);
	virtual ~ContinueStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_CONTINUESTATEMENT_H_ */

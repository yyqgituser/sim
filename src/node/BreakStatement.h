#ifndef SRC_NODE_BREAKSTATEMENT_H_
#define SRC_NODE_BREAKSTATEMENT_H_

#include <memory>
#include "Node.h"

using namespace std;

namespace node {

class BreakStatement: public Node, public enable_shared_from_this<BreakStatement> {
public:
	Location loc;

	BreakStatement(Location loc);
	virtual ~BreakStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_BREAKSTATEMENT_H_ */

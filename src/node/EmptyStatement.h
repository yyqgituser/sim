#ifndef SRC_NODE_EMPTYSTATEMENT_H_
#define SRC_NODE_EMPTYSTATEMENT_H_

#include <memory>
#include "Node.h"

using namespace std;

namespace node {

class EmptyStatement: public Node, public enable_shared_from_this<EmptyStatement> {
public:
	Location loc;

	EmptyStatement(Location loc);
	virtual ~EmptyStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_EMPTYSTATEMENT_H_ */

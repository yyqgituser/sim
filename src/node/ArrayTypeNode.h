#ifndef SRC_NODE_ARRAYTYPENODE_H_
#define SRC_NODE_ARRAYTYPENODE_H_

#include <memory>

#include "Node.h"

using namespace std;

namespace node {

class ArrayTypeNode: public Node, public enable_shared_from_this<ArrayTypeNode> {
public:
	shared_ptr<Node> typeNode;

	ArrayTypeNode(shared_ptr<Node> typeNode);
	virtual ~ArrayTypeNode();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_ARRAYTYPENODE_H_ */

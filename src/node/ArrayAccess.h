#ifndef SRC_NODE_ARRAYACCESS_H_
#define SRC_NODE_ARRAYACCESS_H_

#include <memory>

#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class ArrayAccess: public Node, public enable_shared_from_this<ArrayAccess> {
public:
	shared_ptr<Node> array;

	shared_ptr<Node> index;

	ArrayAccess(shared_ptr<Node> arrExp, shared_ptr<Node> index);
	virtual ~ArrayAccess();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_ARRAYACCESS_H_ */

#ifndef SRC_NODE_ARRAYINITIALIZER_H_
#define SRC_NODE_ARRAYINITIALIZER_H_

#include <memory>
#include <vector>

#include "Node.h"

using namespace std;

namespace node {

class ArrayInitializer: public Node, public enable_shared_from_this<ArrayInitializer> {
public:
	shared_ptr<vector<shared_ptr<Node>>> inits;

	Location loc;

	ArrayInitializer(shared_ptr<vector<shared_ptr<Node>>> inits, Location loc);
	virtual ~ArrayInitializer();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_ARRAYINITIALIZER_H_ */

#ifndef SRC_NODE_TOP_H_
#define SRC_NODE_TOP_H_

#include <memory>
#include <vector>

#include "Node.h"

using namespace std;

namespace node {

class Top: public Node, public enable_shared_from_this<Top> {
public:
	shared_ptr<vector<shared_ptr<Node>>> members;

	Top(shared_ptr<vector<shared_ptr<Node>>> members);
	virtual ~Top();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_TOP_H_ */

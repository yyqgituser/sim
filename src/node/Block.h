#ifndef SRC_NODE_BLOCK_H_
#define SRC_NODE_BLOCK_H_

#include <memory>
#include <vector>

#include "Node.h"

using namespace std;

namespace node {

class Block: public Node, public enable_shared_from_this<Block> {
public:
	shared_ptr<vector<shared_ptr<Node>>> elems;

	Location loc;

	Block(shared_ptr<vector<shared_ptr<Node>>> elems, Location loc);
	virtual ~Block();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_BLOCK_H_ */

#ifndef SRC_NODE_PRIMITIVETYPENODE_H_
#define SRC_NODE_PRIMITIVETYPENODE_H_

#include <memory>

#include "Term.h"

using namespace std;

namespace node {

class PrimitiveTypeNode: public Node, public enable_shared_from_this<PrimitiveTypeNode> {
public:
	shared_ptr<Term> term;

	PrimitiveTypeNode(shared_ptr<Term> term);
	virtual ~PrimitiveTypeNode();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_PRIMITIVETYPENODE_H_ */

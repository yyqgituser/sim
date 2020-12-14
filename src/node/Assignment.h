#ifndef SRC_NODE_ASSIGNMENT_H_
#define SRC_NODE_ASSIGNMENT_H_

#include <memory>
#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class Assignment: public Node, public enable_shared_from_this<Assignment> {
public:
	shared_ptr<Node> left;

	shared_ptr<Node> right;

	shared_ptr<Term> op;

	Assignment(shared_ptr<Node> left, shared_ptr<Node> right, shared_ptr<Term> op);
	virtual ~Assignment();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_ASSIGNMENT_H_ */

#ifndef SRC_NODE_METHODINVOCATION_H_
#define SRC_NODE_METHODINVOCATION_H_

#include <memory>
#include <vector>
#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class MethodInvocation: public Node, public enable_shared_from_this<MethodInvocation> {
public:
	shared_ptr<Term> name;

	shared_ptr<vector<shared_ptr<Node>>> args;

	MethodInvocation(shared_ptr<Term> name, shared_ptr<vector<shared_ptr<Node>>> args);
	virtual ~MethodInvocation();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_METHODINVOCATION_H_ */

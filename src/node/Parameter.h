#ifndef SRC_NODE_PARAMETER_H_
#define SRC_NODE_PARAMETER_H_

#include <memory>

#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class Parameter: public Node, public enable_shared_from_this<Parameter> {
public:
	shared_ptr<Node> typeNode;

	shared_ptr<Term> name;

	Parameter(shared_ptr<Node> typeNode, shared_ptr<Term> name);
	virtual ~Parameter();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_PARAMETER_H_ */

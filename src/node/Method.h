#ifndef SRC_NODE_METHOD_H_
#define SRC_NODE_METHOD_H_

#include <memory>
#include <vector>

#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class Method: public Node, public enable_shared_from_this<Method> {
public:
	shared_ptr<Node> returnType;

	shared_ptr<Term> name;

	shared_ptr<vector<shared_ptr<Node>>> params;

	shared_ptr<Node> body;

	bool isVarParam;

	Method(shared_ptr<Node> returnType, shared_ptr<Term> name, shared_ptr<vector<shared_ptr<Node>>> params,
			shared_ptr<Node> body, bool isVarParam);
	virtual ~Method();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_METHOD_H_ */

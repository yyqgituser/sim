#ifndef SRC_NODE_VARDECL_H_
#define SRC_NODE_VARDECL_H_

#include <memory>

#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class VarDecl: public Node, public enable_shared_from_this<VarDecl> {
public:
	shared_ptr<Node> typeNode;

	shared_ptr<Term> name;

	shared_ptr<Node> init;

	VarDecl(shared_ptr<Node> typeNode, shared_ptr<Term> name, shared_ptr<Node> init);
	virtual ~VarDecl();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_VARDECL_H_ */

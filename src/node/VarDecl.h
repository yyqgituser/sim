#ifndef SRC_NODE_VARDECL_H_
#define SRC_NODE_VARDECL_H_

#include <memory>
#include <vector>

#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class VarDecl: public Node, public enable_shared_from_this<VarDecl> {
public:
	shared_ptr<Node> typeNode;

	shared_ptr<vector<shared_ptr<Node>>> decls;

	VarDecl(shared_ptr<Node> typeNode, shared_ptr<vector<shared_ptr<Node>>> decls);
	virtual ~VarDecl();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_VARDECL_H_ */

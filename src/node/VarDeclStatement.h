#ifndef SRC_NODE_VARDECLSTATEMENT_H_
#define SRC_NODE_VARDECLSTATEMENT_H_

#include <memory>

#include "Node.h"

using namespace std;

namespace node {

class VarDeclStatement: public Node, public enable_shared_from_this<VarDeclStatement> {
public:
	shared_ptr<Node> varDecl;

	VarDeclStatement(shared_ptr<Node> varDecl);
	virtual ~VarDeclStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_VARDECLSTATEMENT_H_ */

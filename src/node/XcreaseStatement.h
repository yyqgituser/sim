#ifndef SRC_NODE_XCREASESTATEMENT_H_
#define SRC_NODE_XCREASESTATEMENT_H_

#include <memory>
#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class XcreaseStatement: public Node, public enable_shared_from_this<XcreaseStatement> {
public:
	shared_ptr<Node> exp;

	shared_ptr<Term> op;

	XcreaseStatement(shared_ptr<Node> exp, shared_ptr<Term> op);
	virtual ~XcreaseStatement();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_XCREASESTATEMENT_H_ */

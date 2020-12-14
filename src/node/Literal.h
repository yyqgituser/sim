#ifndef SRC_NODE_LITERAL_H_
#define SRC_NODE_LITERAL_H_

#include <memory>

#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class Literal: public Node, public enable_shared_from_this<Literal> {
public:
	shared_ptr<Term> term;

	Literal(shared_ptr<Term> term);
	virtual ~Literal();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_LITERAL_H_ */

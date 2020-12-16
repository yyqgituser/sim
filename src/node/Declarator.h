#ifndef SRC_NODE_DECLARATOR_H_
#define SRC_NODE_DECLARATOR_H_

#include <memory>

#include "Node.h"
#include "Term.h"

using namespace std;

namespace node {

class Declarator: public Node, public enable_shared_from_this<Declarator> {
public:
	shared_ptr<Term> name;

	shared_ptr<Node> init;

	Declarator(shared_ptr<Term> name, shared_ptr<Node> init);
	virtual ~Declarator();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_DECLARATOR_H_ */

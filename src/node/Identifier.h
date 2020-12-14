#ifndef SRC_NODE_IDENTIFIER_H_
#define SRC_NODE_IDENTIFIER_H_

#include <memory>

#include "Node.h"
#include "Term.h"

using namespace std;
using namespace parser;

namespace node {

class Identifier: public Node, public enable_shared_from_this<Identifier> {
public:
	shared_ptr<Term> name;

	Identifier(shared_ptr<Term> name);
	virtual ~Identifier();

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

} /* namespace node */

#endif /* SRC_NODE_IDENTIFIER_H_ */

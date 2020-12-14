#ifndef SRC_NODE_TERM_H_
#define SRC_NODE_TERM_H_

#include <memory>

#include "Node.h"
#include "SimToken.h"

using namespace std;
using namespace parser;

namespace node {

class Term: public Node, public enable_shared_from_this<Term> {
public:
	Term(SimToken *token);

	virtual ~Term();

	SimToken *token;

	virtual void accept(NodeVisitor *visitor);

	virtual Location getLocation();
};

}

#endif

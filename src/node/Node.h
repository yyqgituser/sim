#ifndef SRC_NODE_NODE_H_
#define SRC_NODE_NODE_H_

#include <string>

#include "Type.h"
#include "Location.h"

using namespace std;
using namespace type;

namespace node {

class NodeVisitor;

class Node {
public:
	shared_ptr<Type> type;

	Node();

	virtual ~Node();

	virtual void accept(NodeVisitor *visitor) = 0;

	virtual Location getLocation() = 0;
};

}

#endif

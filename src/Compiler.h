#ifndef SRC_COMPILER_H_
#define SRC_COMPILER_H_

#include <memory>

#include "node/Node.h"
#include "Context.h"

using namespace std;
using namespace node;

class Compiler {
public:
	Compiler();
	virtual ~Compiler();

	virtual bool compile(string fileName);

	virtual shared_ptr<Node> parse(string fileName);

	virtual bool attribute(shared_ptr<Node> node, shared_ptr<Context> ctx);

	virtual bool check(shared_ptr<Node> node, shared_ptr<Context> ctx);

	virtual bool gen(string fileName, shared_ptr<Node> node, shared_ptr<Context> ctx);

	virtual void initLLVM();
};

#endif /* SRC_COMPILER_H_ */

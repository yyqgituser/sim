#ifndef SRC_CONTEXT_H_
#define SRC_CONTEXT_H_

#include <memory>
#include <map>

#include "IntType.h"
#include "StringType.h"
#include "DoubleType.h"
#include "BoolType.h"
#include "VoidType.h"
#include "ErrorType.h"

using namespace std;
using namespace type;

class Context {
public:
	shared_ptr<IntType> intType;

	shared_ptr<StringType> stringType;

	shared_ptr<DoubleType> doubleType;

	shared_ptr<BoolType> boolType;

	shared_ptr<VoidType> voidType;

	shared_ptr<ErrorType> errorType;

	Context();
	virtual ~Context();
};

#endif /* SRC_CONTEXT_H_ */

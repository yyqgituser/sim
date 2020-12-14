#include "Context.h"

Context::Context() {
	intType = shared_ptr<IntType>(new IntType());
	stringType = shared_ptr<StringType>(new StringType());
	doubleType = shared_ptr<DoubleType>(new DoubleType());
	boolType = shared_ptr<BoolType>(new BoolType());
	voidType = shared_ptr<VoidType>(new VoidType());
	errorType = shared_ptr<ErrorType>(new ErrorType());
}

Context::~Context() {
}


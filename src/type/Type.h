#ifndef SRC_TYPE_TYPE_H_
#define SRC_TYPE_TYPE_H_

#include <memory>
#include <string>

#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constant.h"

#include "Kind.h"

using namespace std;

namespace type {

class ArrayType;
class MethodType;

class Type {
public:
	Kind kind;

	string name;

	Type(Kind kind, string name);
	virtual ~Type();

	virtual bool isNumer();

	virtual bool equals(shared_ptr<Type> type) = 0;

	virtual bool equalArrayType(shared_ptr<ArrayType> type) = 0;

	virtual bool equalMethodType(shared_ptr<MethodType> type) = 0;

	virtual llvm::Type* getRawType(shared_ptr<llvm::LLVMContext> context) = 0;

	virtual llvm::Constant* getDefaultValue(shared_ptr<llvm::LLVMContext> context) = 0;
};

} /* namespace type */

#endif /* SRC_TYPE_TYPE_H_ */

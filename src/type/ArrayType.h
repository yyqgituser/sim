#ifndef SRC_TYPE_ARRAYTYPE_H_
#define SRC_TYPE_ARRAYTYPE_H_

#include <memory>

#include "Type.h"

using namespace std;

namespace type {

class ArrayType: public Type, public enable_shared_from_this<ArrayType> {
public:
	shared_ptr<Type> elemType;

	ArrayType(shared_ptr<Type> elemType);
	virtual ~ArrayType();

	virtual bool equals(shared_ptr<Type> type);

	virtual bool equalArrayType(shared_ptr<ArrayType> type);

	virtual bool equalMethodType(shared_ptr<MethodType> type);

	virtual llvm::Type *getRawType(shared_ptr<llvm::LLVMContext> context);

	virtual llvm::Constant* getDefaultValue(shared_ptr<llvm::LLVMContext> context);
};

} /* namespace type */

#endif /* SRC_TYPE_ARRAYTYPE_H_ */

#ifndef SRC_TYPE_INTTYPE_H_
#define SRC_TYPE_INTTYPE_H_

#include "Type.h"

namespace type {

class IntType: public Type, public enable_shared_from_this<IntType> {
public:
	IntType();
	virtual ~IntType();

	virtual bool equals(shared_ptr<Type> type);

	virtual bool equalArrayType(shared_ptr<ArrayType> type);

	virtual bool equalMethodType(shared_ptr<MethodType> type);

	virtual llvm::Type *getRawType(shared_ptr<llvm::LLVMContext> context);

	virtual llvm::Constant* getDefaultValue(shared_ptr<llvm::LLVMContext> context);
};

}
/* namespace type */

#endif /* SRC_TYPE_INTTYPE_H_ */

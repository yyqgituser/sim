#ifndef SRC_TYPE_BOOLTYPE_H_
#define SRC_TYPE_BOOLTYPE_H_

#include "Type.h"

namespace type {

class BoolType: public Type, public enable_shared_from_this<BoolType> {
public:
	BoolType();
	virtual ~BoolType();

	virtual bool equals(shared_ptr<Type> type);

	virtual bool equalArrayType(shared_ptr<ArrayType> type);

	virtual bool equalMethodType(shared_ptr<MethodType> type);

	virtual llvm::Type *getRawType(shared_ptr<llvm::LLVMContext> context);

	virtual llvm::Constant* getDefaultValue(shared_ptr<llvm::LLVMContext> context);
};

}
/* namespace type */

#endif /* SRC_TYPE_BOOLTYPE_H_ */

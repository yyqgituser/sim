#ifndef SRC_TYPE_STRINGTYPE_H_
#define SRC_TYPE_STRINGTYPE_H_

#include "Type.h"

namespace type {

class StringType: public Type, public enable_shared_from_this<StringType> {
public:
	StringType();
	virtual ~StringType();

	virtual bool equals(shared_ptr<Type> type);

	virtual bool equalArrayType(shared_ptr<ArrayType> type);

	virtual bool equalMethodType(shared_ptr<MethodType> type);

	virtual llvm::Type *getRawType(shared_ptr<llvm::LLVMContext> context);

	virtual llvm::Constant* getDefaultValue(shared_ptr<llvm::LLVMContext> context);
};

} /* namespace type */

#endif /* SRC_TYPE_STRINGTYPE_H_ */

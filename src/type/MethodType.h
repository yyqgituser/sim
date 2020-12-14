#ifndef SRC_TYPE_METHODTYPE_H_
#define SRC_TYPE_METHODTYPE_H_

#include <memory>
#include <vector>

#include "Type.h"

namespace type {

class MethodType: public Type, public enable_shared_from_this<MethodType> {
public:
	shared_ptr<Type> returnType;

	shared_ptr<vector<shared_ptr<Type>>> paramTypes;

	bool isVarParam;

	MethodType(shared_ptr<Type> returnType, shared_ptr<vector<shared_ptr<Type>>> paramTypes,
			bool isVarParam);
	virtual ~MethodType();

	virtual bool equals(shared_ptr<Type> type);

	virtual bool equalArrayType(shared_ptr<ArrayType> type);

	virtual bool equalMethodType(shared_ptr<MethodType> type);

	virtual llvm::Type *getRawType(shared_ptr<llvm::LLVMContext> context);

	virtual llvm::Constant* getDefaultValue(shared_ptr<llvm::LLVMContext> context);
};

} /* namespace type */

#endif /* SRC_TYPE_METHODTYPE_H_ */

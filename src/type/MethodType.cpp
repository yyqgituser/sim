#include "MethodType.h"

namespace type {

MethodType::MethodType(shared_ptr<Type> returnType, shared_ptr<vector<shared_ptr<Type>>> paramTypes,
		bool isVarParam) :
		Type(METHOD_TYPE, "method"), returnType(returnType), paramTypes(paramTypes), isVarParam(
				isVarParam) {
}

MethodType::~MethodType() {
}

bool MethodType::equals(shared_ptr<Type> type) {
	return type->equalMethodType(shared_from_this());
}

bool MethodType::equalArrayType(shared_ptr<ArrayType> type) {
	return false;
}

bool MethodType::equalMethodType(shared_ptr<MethodType> type) {
	if (!returnType->equals(type->returnType)) {
		return false;
	}
	if (paramTypes->size() != type->paramTypes->size()) {
		return false;
	}
	if (isVarParam != type->isVarParam) {
		return false;
	}
	for (unsigned int i = 0; i < paramTypes->size(); i++) {
		shared_ptr<Type> t = paramTypes->at(i);
		shared_ptr<Type> o = type->paramTypes->at(i);
		if (!t->equals(o)) {
			return false;
		}
	}

	return true;
}

llvm::Type* MethodType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	vector<llvm::Type*> params;
	for (auto &itr : *paramTypes) {
		params.push_back(itr->getRawType(context));
	}
	return llvm::FunctionType::get(returnType->getRawType(context), params, isVarParam);
}

llvm::Constant* MethodType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return 0;
}

} /* namespace type */

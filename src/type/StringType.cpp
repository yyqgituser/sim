#include "StringType.h"

#include "llvm/IR/Constants.h"

namespace type {

StringType::StringType() :
		Type(STRING_TYPE, "string") {
}

StringType::~StringType() {
}

bool StringType::equals(shared_ptr<Type> type) {
	return kind == type->kind;
}

bool StringType::equalArrayType(shared_ptr<ArrayType> type) {
	return false;
}

bool StringType::equalMethodType(shared_ptr<MethodType> type) {
	return false;
}

llvm::Type* StringType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	return llvm::Type::getInt8PtrTy(*context);
}

llvm::Constant* StringType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(*context));
}

} /* namespace type */

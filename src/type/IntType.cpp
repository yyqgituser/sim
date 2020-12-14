#include "IntType.h"

#include "llvm/IR/Constants.h"

namespace type {

IntType::IntType() :
		Type(INT_TYPE, "int") {
}

IntType::~IntType() {
}

bool IntType::equals(shared_ptr<Type> type) {
	return kind == type->kind;
}

bool IntType::equalArrayType(shared_ptr<ArrayType> type) {
	return false;
}

bool IntType::equalMethodType(shared_ptr<MethodType> type) {
	return false;
}

llvm::Type* IntType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	return llvm::Type::getInt32Ty(*context);
}

llvm::Constant* IntType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return llvm::ConstantInt::get(*context, llvm::APInt(32, 0));
}

} /* namespace type */

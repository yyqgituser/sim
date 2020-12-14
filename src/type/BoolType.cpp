#include "BoolType.h"

#include "llvm/IR/Constants.h"

namespace type {

BoolType::BoolType() :
		Type(BOOL_TYPE, "boolean") {
}

BoolType::~BoolType() {
}

bool BoolType::equals(shared_ptr<Type> type) {
	return kind == type->kind;
}

bool BoolType::equalArrayType(shared_ptr<ArrayType> type) {
	return false;
}

bool BoolType::equalMethodType(shared_ptr<MethodType> type) {
	return false;
}

llvm::Type* BoolType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	return llvm::Type::getInt1Ty(*context);
}

llvm::Constant* BoolType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return llvm::ConstantInt::get(*context, llvm::APInt(1, 0));
}

} /* namespace type */

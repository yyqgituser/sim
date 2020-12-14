#include "DoubleType.h"

#include "llvm/IR/Constants.h"

namespace type {

DoubleType::DoubleType() :
		Type(DOUBLE_TYPE, "double") {
}

DoubleType::~DoubleType() {
}

bool DoubleType::equals(shared_ptr<Type> type) {
	return kind == type->kind;
}

bool DoubleType::equalArrayType(shared_ptr<ArrayType> type) {
	return false;
}

bool DoubleType::equalMethodType(shared_ptr<MethodType> type) {
	return false;
}

llvm::Type* DoubleType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	return llvm::Type::getDoubleTy(*context);
}

llvm::Constant* DoubleType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return llvm::ConstantFP::get(*context, llvm::APFloat(0.0));
}

} /* namespace type */

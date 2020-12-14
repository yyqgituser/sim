#include "ArrayType.h"

#include "llvm/IR/Constants.h"

namespace type {

ArrayType::ArrayType(shared_ptr<Type> elemType) :
		Type(ARRAY_TYPE, "array"), elemType(elemType) {
}

ArrayType::~ArrayType() {
}

bool ArrayType::equals(shared_ptr<Type> type) {
	return type->equalArrayType(shared_from_this());
}

bool ArrayType::equalArrayType(shared_ptr<ArrayType> type) {
	return elemType->equals(type->elemType);
}

bool ArrayType::equalMethodType(shared_ptr<MethodType> type) {
	return false;
}

llvm::Type* ArrayType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	return elemType->getRawType(context)->getPointerTo();
}

llvm::Constant* ArrayType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return llvm::ConstantPointerNull::get(elemType->getRawType(context)->getPointerTo());
}

} /* namespace type */

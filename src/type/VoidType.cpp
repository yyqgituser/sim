#include "VoidType.h"

namespace type {

VoidType::VoidType() :
		Type(VOID_TYPE, "void") {
}

VoidType::~VoidType() {
}

bool VoidType::equals(shared_ptr<Type> type) {
	return kind == type->kind;
}

bool VoidType::equalArrayType(shared_ptr<ArrayType> type) {
	return false;
}

bool VoidType::equalMethodType(shared_ptr<MethodType> type) {
	return false;
}

llvm::Type* VoidType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	return llvm::Type::getVoidTy(*context);
}

llvm::Constant* VoidType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return 0;
}

} /* namespace type */

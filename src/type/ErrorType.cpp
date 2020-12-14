#include "ErrorType.h"

namespace type {

ErrorType::ErrorType() :
		Type(ERROR_TYPE, "error") {
}

ErrorType::~ErrorType() {
}

bool ErrorType::equals(shared_ptr<Type> type) {
	return kind == type->kind;
}

bool ErrorType::equalArrayType(shared_ptr<ArrayType> type) {
	return false;
}

bool ErrorType::equalMethodType(shared_ptr<MethodType> type) {
	return false;
}

llvm::Type* ErrorType::getRawType(shared_ptr<llvm::LLVMContext> context) {
	return 0;
}

llvm::Constant* ErrorType::getDefaultValue(shared_ptr<llvm::LLVMContext> context) {
	return 0;
}

} /* namespace type */

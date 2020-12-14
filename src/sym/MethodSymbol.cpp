#include "MethodSymbol.h"

namespace sym {

MethodSymbol::MethodSymbol(u32string name, shared_ptr<type::Type> type) :
		Symbol(name, type) {
}

MethodSymbol::~MethodSymbol() {
}

} /* namespace sym */

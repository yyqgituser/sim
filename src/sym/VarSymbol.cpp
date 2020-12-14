#include "VarSymbol.h"

namespace sym {

VarSymbol::VarSymbol(u32string name, shared_ptr<type::Type> type) :
		Symbol(name, type), value(0) {
}

VarSymbol::~VarSymbol() {
}

} /* namespace sym */

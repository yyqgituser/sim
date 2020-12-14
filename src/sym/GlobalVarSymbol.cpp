#include "GlobalVarSymbol.h"

namespace sym {

GlobalVarSymbol::GlobalVarSymbol(u32string name, shared_ptr<type::Type> type) :
		Symbol(name, type) {
}

GlobalVarSymbol::~GlobalVarSymbol() {
}

} /* namespace sym */

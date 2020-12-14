#include "Symbol.h"

namespace sym {

Symbol::Symbol(u32string name, shared_ptr<type::Type> type) :
		name(name), type(type) {
}

Symbol::~Symbol() {
	// TODO Auto-generated destructor stub
}

} /* namespace sym */

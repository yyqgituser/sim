#ifndef SRC_SYM_SYMBOL_H_
#define SRC_SYM_SYMBOL_H_

#include <memory>
#include <string>

#include "Type.h"

using namespace std;
using namespace type;

namespace sym {

class Symbol {
public:
	enum SymbolKind {
		VAR, GVAR, METHOD
	};

	SymbolKind kind;

	u32string name;

	shared_ptr<Type> type;

	Symbol(u32string name, shared_ptr<type::Type> type);
	virtual ~Symbol();
};

} /* namespace sym */

#endif /* SRC_SYM_SYMBOL_H_ */

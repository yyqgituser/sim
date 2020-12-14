#ifndef SRC_SYM_GLOBALVARSYMBOL_H_
#define SRC_SYM_GLOBALVARSYMBOL_H_

#include "Symbol.h"

#include "llvm/IR/GlobalVariable.h"

namespace sym {

class GlobalVarSymbol: public Symbol {
public:
	shared_ptr<llvm::GlobalVariable> var;

	GlobalVarSymbol(u32string name, shared_ptr<type::Type> type);
	virtual ~GlobalVarSymbol();
};

} /* namespace sym */

#endif /* SRC_SYM_GLOBALVARSYMBOL_H_ */

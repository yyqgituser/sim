#ifndef SRC_SYM_METHODSYMBOL_H_
#define SRC_SYM_METHODSYMBOL_H_

#include "Symbol.h"

#include "llvm/IR/Function.h"

namespace sym {

class MethodSymbol: public Symbol {
public:
	llvm::Function *value;

	MethodSymbol(u32string name, shared_ptr<type::Type> type);
	virtual ~MethodSymbol();
};

} /* namespace sym */

#endif /* SRC_SYM_METHODSYMBOL_H_ */

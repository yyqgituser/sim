#ifndef SRC_SYM_VARSYMBOL_H_
#define SRC_SYM_VARSYMBOL_H_

#include "Symbol.h"

#include "llvm/IR/Instructions.h"

namespace sym {

class VarSymbol: public Symbol {
public:
	llvm::Value *value; // local: AllocaInst global: GlobalVariable

	VarSymbol(u32string name, shared_ptr<type::Type> type);
	virtual ~VarSymbol();
};

} /* namespace sym */

#endif /* SRC_SYM_VARSYMBOL_H_ */

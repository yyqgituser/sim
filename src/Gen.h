#ifndef SRC_GEN_H_
#define SRC_GEN_H_

#include <memory>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#include "NodeVisitor.h"
#include "Context.h"
#include "Scope.h"
#include "Symbol.h"
#include "SimTokenType.h"

using namespace node;
using namespace sym;
using namespace llvm;
using namespace parser;

class Gen: public NodeVisitor {
public:
	string filename;

	shared_ptr<Context> simCtx;

	int result;

	Gen(string filename, shared_ptr<Context> simCtx);
	virtual ~Gen();

	virtual void visitLiteral(shared_ptr<Literal> node);

	virtual void visitPrimitiveTypeNode(shared_ptr<PrimitiveTypeNode> node);

	virtual void visitArrayTypeNode(shared_ptr<ArrayTypeNode> node);

	virtual void visitTerm(shared_ptr<Term> node);

	virtual void visitVarDecl(shared_ptr<VarDecl> node);

	virtual void visitTop(shared_ptr<Top> node);

	virtual void visitMethod(shared_ptr<Method> node);

	virtual void visitParameter(shared_ptr<Parameter> node);

	virtual void visitArrayInitializer(shared_ptr<ArrayInitializer> node);

	virtual void visitBlock(shared_ptr<Block> node);

	virtual void visitVarDeclStatement(shared_ptr<VarDeclStatement> node);

	virtual void visitExpressionStatement(shared_ptr<ExpressionStatement> node);

	virtual void visitIfStatement(shared_ptr<IfStatement> node);

	virtual void visitWhileStatement(shared_ptr<WhileStatement> node);

	virtual void visitBreakStatement(shared_ptr<BreakStatement> node);

	virtual void visitContinueStatement(shared_ptr<ContinueStatement> node);

	virtual void visitReturnStatement(shared_ptr<ReturnStatement> node);

	virtual void visitBinaryExpression(shared_ptr<BinaryExpression> node);

	virtual void visitUnaryExpression(shared_ptr<UnaryExpression> node);

	virtual void visitCastExpression(shared_ptr<CastExpression> node);

	virtual void visitIdentifier(shared_ptr<Identifier> node);

	virtual void visitArrayAccess(shared_ptr<ArrayAccess> node);

	virtual void visitMethodInvocation(shared_ptr<MethodInvocation> node);

	virtual void visitAssignment(shared_ptr<Assignment> node);

	virtual void visitEmptyStatement(shared_ptr<EmptyStatement> node);

	virtual void visitXcreaseStatement(shared_ptr<XcreaseStatement> node);

	virtual void visitForStatement(shared_ptr<ForStatement> node);

private:
	shared_ptr<LLVMContext> context;

	shared_ptr<Module> module;

	shared_ptr<IRBuilder<>> builder;

	shared_ptr<type::Type> returnType;

	shared_ptr<Scope<sym::Symbol>> global;

	shared_ptr<Scope<sym::Symbol>> local;

	shared_ptr<vector<shared_ptr<type::Type>>> paramTypes;

	bool isLocal;

	Function *function;

	Argument *argument;

	BasicBlock *block;

	Value *value;

	bool reachable; // is current block returned;

	bool breaked;

	bool isLeftValue;

	BasicBlock *loopContBlock;

	BasicBlock *loopEndBlock;

	void createAnonymousInitFunction();

	AllocaInst* createAlloca(llvm::Type *type, StringRef VarName);

	CmpInst::Predicate tokenTypeToCmpPredicate(bool isFloat, SimTokenType id);
};

#endif /* SRC_GEN_H_ */

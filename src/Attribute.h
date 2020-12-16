#ifndef SRC_ATTRIBUTE_H_
#define SRC_ATTRIBUTE_H_

#include <memory>

#include "NodeVisitor.h"
#include "Context.h"
#include "Scope.h"

using namespace node;

class Attribute: public NodeVisitor {
public:
	shared_ptr<Context> ctx;

	int result;

	Attribute(shared_ptr<Context> ctx);
	virtual ~Attribute();

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

	virtual void visitDeclarator(shared_ptr<Declarator> node);

private:
	shared_ptr<Scope<Type>> local;

	shared_ptr<VarDecl> varDeclNode;

	shared_ptr<vector<shared_ptr<Type>>> paramTypes;

	void printBinaryError(shared_ptr<Term> op, shared_ptr<Type> left, shared_ptr<Type> right);

	void printUnaryError(shared_ptr<Term> op, shared_ptr<Type> expType);
};

#endif /* SRC_ATTRIRBUTE_H_ */

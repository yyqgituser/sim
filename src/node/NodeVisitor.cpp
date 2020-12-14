#include "NodeVisitor.h"

namespace node {

NodeVisitor::NodeVisitor() {
}

NodeVisitor::~NodeVisitor() {
}

void NodeVisitor::visitLiteral(shared_ptr<Literal> node) {
}

void NodeVisitor::visitPrimitiveTypeNode(shared_ptr<PrimitiveTypeNode> node) {
}

void NodeVisitor::visitArrayTypeNode(shared_ptr<ArrayTypeNode> node) {
}

void NodeVisitor::visitTerm(shared_ptr<Term> node) {
}

void NodeVisitor::visitVarDecl(shared_ptr<VarDecl> node) {
}

void NodeVisitor::visitTop(shared_ptr<Top> node) {
}

void NodeVisitor::visitMethod(shared_ptr<Method> node) {
}

void NodeVisitor::visitParameter(shared_ptr<Parameter> node) {
}

void NodeVisitor::visitArrayInitializer(shared_ptr<ArrayInitializer> node) {
}

void NodeVisitor::visitBlock(shared_ptr<Block> node) {
}

void NodeVisitor::visitVarDeclStatement(shared_ptr<VarDeclStatement> node) {
}

void NodeVisitor::visitExpressionStatement(shared_ptr<ExpressionStatement> node) {
}

void NodeVisitor::visitIfStatement(shared_ptr<IfStatement> node) {
}

void NodeVisitor::visitWhileStatement(shared_ptr<WhileStatement> node) {
}

void NodeVisitor::visitBreakStatement(shared_ptr<BreakStatement> node) {
}

void NodeVisitor::visitContinueStatement(shared_ptr<ContinueStatement> node) {
}

void NodeVisitor::visitReturnStatement(shared_ptr<ReturnStatement> node) {
}

void NodeVisitor::visitBinaryExpression(shared_ptr<BinaryExpression> node) {
}

void NodeVisitor::visitUnaryExpression(shared_ptr<UnaryExpression> node) {
}

void NodeVisitor::visitCastExpression(shared_ptr<CastExpression> node) {
}

void NodeVisitor::visitIdentifier(shared_ptr<Identifier> node) {
}

void NodeVisitor::visitArrayAccess(shared_ptr<ArrayAccess> node) {
}

void NodeVisitor::visitMethodInvocation(shared_ptr<MethodInvocation> node) {
}

void NodeVisitor::visitAssignment(shared_ptr<Assignment> node) {
}

void NodeVisitor::visitEmptyStatement(shared_ptr<EmptyStatement> node) {
}

void NodeVisitor::visitXcreaseStatement(shared_ptr<XcreaseStatement> node) {
}

void NodeVisitor::visitForStatement(shared_ptr<ForStatement> node) {
}

}


#include "SimSemanticorImpl.h"

#include <iostream>
#include <string>
#include <cmath>
#include <codecvt>
#include <locale>
#include <stack>

#include "misc.h"
#include "SimSemanticor.h"
#include "SimToken.h"
#include "Literal.h"
#include "PrimitiveTypeNode.h"
#include "ArrayTypeNode.h"
#include "VarDecl.h"
#include "Method.h"
#include "Parameter.h"
#include "ArrayInitializer.h"
#include "Block.h"
#include "VarDeclStatement.h"
#include "ExpressionStatement.h"
#include "IfStatement.h"
#include "WhileStatement.h"
#include "BreakStatement.h"
#include "ContinueStatement.h"
#include "ReturnStatement.h"
#include "BinaryExpression.h"
#include "UnaryExpression.h"
#include "CastExpression.h"
#include "Identifier.h"
#include "ArrayAccess.h"
#include "MethodInvocation.h"
#include "Assignment.h"
#include "EmptyStatement.h"
#include "XcreaseStatement.h"
#include "ForStatement.h"

using namespace util;

SimSemanticorImpl::SimSemanticorImpl() :
		isVarParam(false) {
	members = shared_ptr<vector<shared_ptr<Node>>>(new vector<shared_ptr<Node>>());
}

SimSemanticorImpl::~SimSemanticorImpl() {
}

void SimSemanticorImpl::push(shared_ptr<Term> node) {
	terms.push(node);
}

shared_ptr<Term> SimSemanticorImpl::pop() {
	shared_ptr<Term> t = terms.top();
	terms.pop();
	return t;
}

void SimSemanticorImpl::pushNode(shared_ptr<Node> node) {
	nodes.push(node);
}

shared_ptr<Node> SimSemanticorImpl::popNode() {
	shared_ptr<Node> t = nodes.top();
	nodes.pop();
	return t;
}

void SimSemanticorImpl::pushList(shared_ptr<vector<shared_ptr<Node>>> list) {
	lists.push(list);
}

shared_ptr<vector<shared_ptr<Node>>> SimSemanticorImpl::popList() {
	shared_ptr<vector<shared_ptr<Node>>> t = lists.top();
	lists.pop();
	return t;
}

shared_ptr<Node> SimSemanticorImpl::getAst() {
	return move(top);
}

void SimSemanticorImpl::outToken(SimToken *token) {
	push(shared_ptr<Term>(new Term(token)));

}

bool SimSemanticorImpl::declLiteral() {
	shared_ptr<Term> term = pop();
	pushNode(shared_ptr<Literal>(new Literal(move(term))));
	return true;
}

bool SimSemanticorImpl::declPrimitiveType() {
	pushNode(shared_ptr<PrimitiveTypeNode>(new PrimitiveTypeNode(pop())));
	return true;
}

bool SimSemanticorImpl::declArrayType() {
	pop();
	pop();
	pushNode(shared_ptr<ArrayTypeNode>(new ArrayTypeNode(popNode())));
	return true;
}

bool SimSemanticorImpl::declTop() {
	top = shared_ptr<Top>(new Top(move(members)));
	return true;
}

bool SimSemanticorImpl::declTopVar() {
	pop();
	members->push_back(popNode());
	return true;
}

bool SimSemanticorImpl::declVarWithoutInit() {
	pushNode(shared_ptr<Node>(new VarDecl(popNode(), pop(), 0)));
	return true;
}

bool SimSemanticorImpl::declVarWithInit() {
	shared_ptr<Node> init = popNode();
	pop();
	shared_ptr<Term> name = pop();
	shared_ptr<Node> type = popNode();
	pushNode(shared_ptr<Node>(new VarDecl(type, name, init)));
	return true;
}

bool SimSemanticorImpl::declMethodWithoutBody() {
	pop()->token->text;
	pop()->token->text;
	pop()->token->text;
	shared_ptr<Term> name = pop();
	shared_ptr<Node> type = popNode();
	shared_ptr<Method> method = shared_ptr<Method>(
			new Method(move(type), move(name), popList(), 0, isVarParam));
	isVarParam = false;
	members->push_back(move(method));
	return true;
}

bool SimSemanticorImpl::declMethodWithBody() {
	shared_ptr<Node> body = popNode();
	shared_ptr<Term> rp = pop();
	shared_ptr<Term> lp = pop();
	shared_ptr<Term> name = pop();
	shared_ptr<Node> type = popNode();
	shared_ptr<Method> method = shared_ptr<Method>(
			new Method(move(type), move(name), popList(), move(body), isVarParam));
	isVarParam = false;
	members->push_back(move(method));
	return true;
}

bool SimSemanticorImpl::declEmptyParams() {
	pushList(shared_ptr<vector<shared_ptr<Node>>>(new vector<shared_ptr<Node>>()));
	return true;
}

bool SimSemanticorImpl::firstParam() {
	shared_ptr<vector<shared_ptr<Node>>> params = shared_ptr<vector<shared_ptr<Node>>>(
			new vector<shared_ptr<Node>>());
	params->push_back(popNode());
	pushList(move(params));
	return true;
}

bool SimSemanticorImpl::nextParam() {
	pop();
	lists.top()->push_back(popNode());
	return true;
}

bool SimSemanticorImpl::declVarParam() {
	pop();
	isVarParam = true;
	return true;
}

bool SimSemanticorImpl::declVarParam1() {
	pop();
	pop();
	isVarParam = true;
	return true;
}

bool SimSemanticorImpl::declParam() {
	shared_ptr<Term> paramName = pop();
	pushNode(shared_ptr<Parameter>(new Parameter(popNode(), paramName)));
	return true;
}

bool SimSemanticorImpl::declEmptyArrayInitializer() {
	pop();
	shared_ptr<Term> brace = pop();
	shared_ptr<vector<shared_ptr<Node>>> inits = shared_ptr<vector<shared_ptr<Node>>>(
			new vector<shared_ptr<Node>>());
	pushNode(
			shared_ptr<ArrayInitializer>(
					new ArrayInitializer(move(inits),
							Location(brace->token->line, brace->token->column))));
	return true;
}

bool SimSemanticorImpl::declArrayInitializer() {
	pop();
	shared_ptr<Term> brace = pop();
	pushNode(
			shared_ptr<ArrayInitializer>(
					new ArrayInitializer(popList(),
							Location(brace->token->line, brace->token->column))));
	return true;
}

bool SimSemanticorImpl::firstVariableInitializer() {
	shared_ptr<vector<shared_ptr<Node>>> inits = shared_ptr<vector<shared_ptr<Node>>>(
			new vector<shared_ptr<Node>>());
	inits->push_back(popNode());
	pushList(move(inits));
	return true;
}

bool SimSemanticorImpl::nextVariableInitializer() {
	pop();
	lists.top()->push_back(popNode());
	return true;
}

bool SimSemanticorImpl::declEmptyBlock() {
	pop();
	shared_ptr<Term> brace = pop();
	shared_ptr<vector<shared_ptr<Node>>> elems = shared_ptr<vector<shared_ptr<Node>>>(
			new vector<shared_ptr<Node>>());
	pushNode(
			shared_ptr<Node>(
					new Block(move(elems), Location(brace->token->line, brace->token->column))));
	return true;
}

bool SimSemanticorImpl::declBlock() {
	pop();
	shared_ptr<Term> brace = pop();
	pushNode(
			shared_ptr<Node>(
					new Block(popList(), Location(brace->token->line, brace->token->column))));
	return true;
}

bool SimSemanticorImpl::firstBlockStatement() {
	shared_ptr<vector<shared_ptr<Node>>> list = shared_ptr<vector<shared_ptr<Node>>>(
			new vector<shared_ptr<Node>>());
	list->push_back(popNode());
	pushList(move(list));
	return true;
}

bool SimSemanticorImpl::nextBlockStatement() {
	lists.top()->push_back(popNode());
	return true;
}

bool SimSemanticorImpl::declLocalVar() {
	pop();
	pushNode(shared_ptr<VarDeclStatement>(new VarDeclStatement(popNode())));
	return true;
}

bool SimSemanticorImpl::declEmptyStatement() {
	shared_ptr<Term> term = pop();
	pushNode(shared_ptr<EmptyStatement>(new EmptyStatement(term->getLocation())));
	return true;
}

bool SimSemanticorImpl::declExpressionStatement() {
	pop();
	pushNode(shared_ptr<ExpressionStatement>(new ExpressionStatement(popNode())));
	return true;
}

bool SimSemanticorImpl::declIfStatement() {
	pop();
	pop();
	shared_ptr<Term> ifTerm = pop();
	shared_ptr<Node> thenStmt = popNode();
	shared_ptr<Node> condExp = popNode();
	pushNode(
			shared_ptr<IfStatement>(
					new IfStatement(move(condExp), move(thenStmt), 0,
							Location(ifTerm->token->line, ifTerm->token->column))));
	return true;
}

bool SimSemanticorImpl::declIfElseStatement() {
	pop();
	pop();
	pop();
	shared_ptr<Term> ifTerm = pop();
	shared_ptr<Node> elseStmt = popNode();
	shared_ptr<Node> thenStmt = popNode();
	shared_ptr<Node> condExp = popNode();
	pushNode(
			shared_ptr<IfStatement>(
					new IfStatement(move(condExp), move(thenStmt), move(elseStmt),
							Location(ifTerm->token->line, ifTerm->token->column))));
	return true;
}

bool SimSemanticorImpl::declWhileStatement() {
	pop();
	pop();
	shared_ptr<Term> whileTerm = pop();
	shared_ptr<Node> body = popNode();
	shared_ptr<Node> condExp = popNode();
	pushNode(
			shared_ptr<WhileStatement>(
					new WhileStatement(move(condExp), move(body),
							Location(whileTerm->token->line, whileTerm->token->column))));
	return true;
}

bool SimSemanticorImpl::declBreakStatement() {
	pop();
	shared_ptr<Term> term = pop();
	pushNode(shared_ptr<BreakStatement>(new BreakStatement(term->getLocation())));
	return true;
}

bool SimSemanticorImpl::declContinueStatement() {
	pop();
	shared_ptr<Term> term = pop();
	pushNode(shared_ptr<ContinueStatement>(new ContinueStatement(term->getLocation())));
	return true;
}

bool SimSemanticorImpl::declReturnWithoutExpression() {
	pop();
	shared_ptr<Term> term = pop();
	pushNode(shared_ptr<ReturnStatement>(new ReturnStatement(term->getLocation())));
	return true;
}

bool SimSemanticorImpl::declReturnWithExpression() {
	pop();
	shared_ptr<Term> term = pop();
	pushNode(shared_ptr<ReturnStatement>(new ReturnStatement(popNode(), term->getLocation())));
	return true;
}

bool SimSemanticorImpl::declBinaryExpr() {
	shared_ptr<Term> op = pop();
	shared_ptr<Node> right = popNode();
	shared_ptr<Node> left = popNode();
	pushNode(shared_ptr<BinaryExpression>(new BinaryExpression(op, left, right)));
	return true;
}

bool SimSemanticorImpl::declPrefixExpression() {
	shared_ptr<Term> op = pop();
	shared_ptr<Node> exp = popNode();
	pushNode(shared_ptr<UnaryExpression>(new UnaryExpression(op, exp)));
	return true;
}

bool SimSemanticorImpl::declCastExpression() {
	pop();
	pop();
	shared_ptr<Node> exp = popNode();
	shared_ptr<Node> type = popNode();
	pushNode(shared_ptr<CastExpression>(new CastExpression(type, exp)));
	return true;
}

bool SimSemanticorImpl::declParanExpression() {
	pop();
	pop();
	return true;
}

bool SimSemanticorImpl::declIdentifier() {
	shared_ptr<Term> name = pop();
	pushNode(shared_ptr<Identifier>(new Identifier(name)));
	return true;
}

bool SimSemanticorImpl::declArrayAccess() {
	pop();
	pop();
	shared_ptr<Node> index = popNode();
	shared_ptr<Node> arr = popNode();
	pushNode(shared_ptr<ArrayAccess>(new ArrayAccess(arr, index)));
	return true;
}

bool SimSemanticorImpl::declMethodInvocation() {
	pop();
	pop();
	shared_ptr<Term> name = pop();
	shared_ptr<vector<shared_ptr<Node>>> args = popList();
	pushNode(shared_ptr<MethodInvocation>(new MethodInvocation(name, args)));
	return true;
}

bool SimSemanticorImpl::declEmptyArgumentList() {
	pushList(shared_ptr<vector<shared_ptr<Node>>>(new vector<shared_ptr<Node>>()));
	return true;
}

bool SimSemanticorImpl::firstArgument() {
	shared_ptr<vector<shared_ptr<Node>>> params = shared_ptr<vector<shared_ptr<Node>>>(
			new vector<shared_ptr<Node>>());
	params->push_back(popNode());
	pushList(move(params));
	return true;
}

bool SimSemanticorImpl::nextArgument() {
	pop();
	lists.top()->push_back(popNode());
	return true;
}

bool SimSemanticorImpl::declAssignment() {
	shared_ptr<Term> op = pop();
	shared_ptr<Node> right = popNode();
	shared_ptr<Node> left = popNode();
	pushNode(shared_ptr<Assignment>(new Assignment(left, right, op)));
	return true;
}

bool SimSemanticorImpl::declXcreaseStatement() {
	shared_ptr<Term> op = pop();
	shared_ptr<Node> exp = popNode();
	pushNode(shared_ptr<XcreaseStatement>(new XcreaseStatement(exp, op)));
	return true;
}

bool SimSemanticorImpl::declForStatement() {
	pop();
	pop();
	pop();
	pop();
	shared_ptr<Term> forTerm = pop();
	shared_ptr<Node> body = popNode();
	shared_ptr<Node> update = popNode();
	shared_ptr<Node> condExp = popNode();
	shared_ptr<Node> init = popNode();
	pushNode(
			make_shared<ForStatement>(move(init), move(condExp), move(update), move(body),
					forTerm->getLocation()));
	return true;
}

bool SimSemanticorImpl::declEmptyForInit() {
	pushNode(0);
	return true;
}

bool SimSemanticorImpl::declEmptyForCondition() {
	pushNode(0);
	return true;
}

bool SimSemanticorImpl::declEmptyForUpdate() {
	pushNode(0);
	return true;
}


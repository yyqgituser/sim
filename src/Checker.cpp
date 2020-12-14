#include "Checker.h"

#include <iostream>

#include "SimTokenType.h"
#include "ArrayType.h"
#include "MethodType.h"
#include "misc.h"
#include "Scope.h"

using namespace std;
using namespace parser;
using namespace type;
using namespace util;

Checker::Checker(shared_ptr<Context> ctx) :
		ctx(ctx), result(0), local(new Scope<Type>(0)) {
}

Checker::~Checker() {
}

void Checker::visitLiteral(shared_ptr<Literal> node) {
}

void Checker::visitPrimitiveTypeNode(shared_ptr<PrimitiveTypeNode> node) {
}

void Checker::visitArrayTypeNode(shared_ptr<ArrayTypeNode> node) {
}

void Checker::visitTerm(shared_ptr<Term> node) {
}

void Checker::printAssignError(Location loc, shared_ptr<Type> left, shared_ptr<Type> right) {
	cout << loc.line << " line, " << loc.column << " column, error: can't assign value of type "
			<< right->name << " to " << left->name << endl;
}

void Checker::visitVarDecl(shared_ptr<VarDecl> node) {
	if (node->init) {
		if (!isAssignable(node->typeNode->type, node->init->type)) {
			printAssignError(node->getLocation(), node->typeNode->type, node->init->type);
			result = 1;
			return;
		}
		node->init->accept(this);
	}
}

void Checker::visitTop(shared_ptr<Top> node) {
	for (vector<shared_ptr<Node>>::iterator itr = node->members->begin();
			itr != node->members->end(); ++itr) {
		(*itr)->accept(this);
	}
}

void Checker::visitMethod(shared_ptr<Method> node) {
	u32string name = node->name->token->text;
	local->add(name, node->type);
	shared_ptr<Scope<Type>> outScope = move(local);
	local = shared_ptr<Scope<Type>>(new Scope<Type>(outScope));
	if (node->body) {
		returnType = node->returnType->type;
		node->body->accept(this);
		returnType = 0;
	}
	local = outScope;
}

void Checker::visitParameter(shared_ptr<Parameter> node) {
	u32string name = node->name->token->text;
	local->add(name, node->typeNode->type);
}

void Checker::visitArrayInitializer(shared_ptr<ArrayInitializer> node) {
}

void Checker::visitBlock(shared_ptr<Block> node) {
	shared_ptr<Scope<Type>> outScope = move(local);
	local = shared_ptr<Scope<Type>>(new Scope<Type>(outScope));
	for (vector<shared_ptr<Node>>::iterator itr = node->elems->begin(); itr != node->elems->end();
			++itr) {
		(*itr)->accept(this);
	}
	local = outScope;
}

void Checker::visitVarDeclStatement(shared_ptr<VarDeclStatement> node) {
	node->varDecl->accept(this);
}

void Checker::visitExpressionStatement(shared_ptr<ExpressionStatement> node) {
	node->exp->accept(this);
}

void Checker::visitIfStatement(shared_ptr<IfStatement> node) {
	if (node->condExp->type->kind != BOOL_TYPE) {
		Location loc = node->condExp->getLocation();
		cout << loc.line << " line, " << loc.column
				<< " column, error: the type of condition expression of if statement must be boolean"
				<< endl;
		result = 1;
		return;
	}
	node->condExp->accept(this);
	node->thenStmt->accept(this);
	if (node->elseStmt) {
		node->elseStmt->accept(this);
	}
}

void Checker::visitWhileStatement(shared_ptr<WhileStatement> node) {
	if (node->condExp->type->kind != BOOL_TYPE) {
		Location loc = node->condExp->getLocation();
		cout << loc.line << " line, " << loc.column
				<< " column, error: the type of condition expression of if statement must be boolean"
				<< endl;
		result = 1;
		return;
	}
	node->condExp->accept(this);
	node->body->accept(this);
}

void Checker::visitBreakStatement(shared_ptr<BreakStatement> node) {
}

void Checker::visitContinueStatement(shared_ptr<ContinueStatement> node) {
}

void Checker::visitReturnStatement(shared_ptr<ReturnStatement> node) {
	if (node->exp) {
		if (returnType->kind == VOID_TYPE) {
			Location loc = node->exp->getLocation();
			cout << loc.line << " line, " << loc.column
					<< " column, error: can't return a value from a void function" << endl;
			result = 1;
			return;
		}
		if (!isAssignable(returnType, node->exp->type)) {
			printAssignError(node->exp->getLocation(), returnType, node->exp->type);
			result = 1;
			return;
		}
		node->exp->accept(this);
	} else {
		if (returnType->kind != VOID_TYPE) {
			Location loc = node->getLocation();
			cout << loc.line << " line, " << loc.column
					<< " column, error: can't return without a value from a non-void function"
					<< endl;
			result = 1;
			return;
		}
	}
}

void Checker::printBinaryError(shared_ptr<Term> op, shared_ptr<Type> left, shared_ptr<Type> right) {
	cout << op->token->line << " line, " << op->token->column
			<< " column, error: can't apply type of " << left->name << " and " << right->name
			<< " to binary operator " << toUtf8(op->token->text) << endl;
}

void Checker::printUnaryError(shared_ptr<Term> op, shared_ptr<Type> expType) {
	cout << op->token->line << " line, " << op->token->column
			<< " column, error: can't apply type of " << expType->name << " to unary operator "
			<< toUtf8(op->token->text) << endl;
}

void Checker::visitBinaryExpression(shared_ptr<BinaryExpression> node) {
	node->left->accept(this);
	node->right->accept(this);
}

void Checker::visitUnaryExpression(shared_ptr<UnaryExpression> node) {
	node->exp->accept(this);
}

void Checker::visitCastExpression(shared_ptr<CastExpression> node) {
	node->exp->accept(this);
}

void Checker::visitIdentifier(shared_ptr<Identifier> node) {
}

void Checker::visitArrayAccess(shared_ptr<ArrayAccess> node) {
	node->index->accept(this);
	if (node->index->type->kind != INT_TYPE) {
		Location loc = node->getLocation();
		cout << loc.line << " line, " << loc.column << " column, error: can't use value of "
				<< node->index->type->name << " as array index" << endl;
		result = 1;
		return;
	}
	node->array->accept(this);
}

void Checker::visitMethodInvocation(shared_ptr<MethodInvocation> node) {
	u32string name = node->name->token->text;
	shared_ptr<MethodType> methodType = dynamic_pointer_cast<MethodType>(local->lookup(name));
	if ((methodType->isVarParam && node->args->size() < methodType->paramTypes->size())
			|| ((!methodType->isVarParam) && node->args->size() != methodType->paramTypes->size())) {
		Location loc = node->getLocation();
		cout << loc.line << " line, " << loc.column << " column, error: function '" << toUtf8(name)
				<< "' with " << methodType->paramTypes->size()
				<< " parameters can't be called with " << node->args->size() << " arguments"
				<< endl;
		result = 1;
		return;
	}
	for (unsigned int i = 0; i < methodType->paramTypes->size(); i++) {
		shared_ptr<Type> left = methodType->paramTypes->at(i);
		shared_ptr<Type> right = node->args->at(i)->type;
		if (!isAssignable(left, right)) {
			Location loc = node->args->at(i)->getLocation();
			printAssignError(loc, left, right);
			result = 1;
			return;
		}
	}

}

void Checker::visitAssignment(shared_ptr<Assignment> node) {
	shared_ptr<Type> left = node->left->type;
	shared_ptr<Type> right = node->right->type;
	if (!isAssignable(left, right)) {
		Location loc = node->getLocation();
		printAssignError(loc, left, right);
		result = 1;
		return;
	}
	node->right->accept(this);
}

void Checker::visitEmptyStatement(shared_ptr<EmptyStatement> node) {
}

void Checker::visitXcreaseStatement(shared_ptr<XcreaseStatement> node) {
}

void Checker::visitForStatement(shared_ptr<ForStatement> node) {
	if (node->init) {
		node->init->accept(this);
		if (result != 0) {
			return;
		}
	}
	if (node->condExp) {
		if (node->condExp->type->kind != BOOL_TYPE) {
			Location loc = node->condExp->getLocation();
			cout << loc.line << " line, " << loc.column
					<< " column, error: the type of condition expression of if statement must be boolean"
					<< endl;
			result = 1;
			return;
		}
		node->condExp->accept(this);
		if (result != 0) {
			return;
		}
	}
	if (node->update) {
		node->update->accept(this);
		if (result != 0) {
			return;
		}
	}
}

bool Checker::isAssignable(shared_ptr<Type> left, shared_ptr<Type> right) {
	if (left->kind == BOOL_TYPE || left->kind == INT_TYPE || left->kind == STRING_TYPE) {
		return right->kind == left->kind;
	} else if (left->kind == DOUBLE_TYPE) {
		return right->kind == DOUBLE_TYPE || right->kind == INT_TYPE;
	} else if (left->kind == ARRAY_TYPE) {
		return left->equals(right);
	} else {
		return false;
	}
}


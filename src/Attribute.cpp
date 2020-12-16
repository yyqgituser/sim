#include "Attribute.h"

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

Attribute::Attribute(shared_ptr<Context> ctx) :
		ctx(ctx), result(0), local(new Scope<Type>(0)) {
}

Attribute::~Attribute() {
}

void Attribute::visitLiteral(shared_ptr<Literal> node) {
	switch (node->term->token->id) {
	case INT_LITERAL:
		node->type = ctx->intType;
		break;
	case DOUBLE_LITERAL:
		node->type = ctx->doubleType;
		break;
	case STRING_LITERAL:
		node->type = ctx->stringType;
		break;
	case TRUE:
		node->type = ctx->boolType;
		break;
	case FALSE:
		node->type = ctx->boolType;
		break;
	}
}

void Attribute::visitPrimitiveTypeNode(shared_ptr<PrimitiveTypeNode> node) {
	switch (node->term->token->id) {
	case INT:
		node->type = ctx->intType;
		break;
	case DOUBLE:
		node->type = ctx->doubleType;
		break;
	case STRING:
		node->type = ctx->stringType;
		break;
	case BOOLEAN:
		node->type = ctx->boolType;
		break;
	case VOID:
		node->type = ctx->voidType;
		break;
	}
}

void Attribute::visitArrayTypeNode(shared_ptr<ArrayTypeNode> node) {
	node->typeNode->accept(this);
	if (node->typeNode->type->kind == ERROR_TYPE) {
		node->type = ctx->errorType; // always give a type
		return;
	}
	node->type = shared_ptr<ArrayType>(new ArrayType(node->typeNode->type));
}

void Attribute::visitTerm(shared_ptr<Term> node) {
}

void Attribute::visitVarDecl(shared_ptr<VarDecl> node) {
	node->typeNode->accept(this);
	if (node->typeNode->type->kind == ERROR_TYPE) {
		return;
	}
	varDeclNode = node;
	for (auto &decl : *node->decls) {
		decl->accept(this);
		if (result != 0) {
			return;
		}
	}
	varDeclNode = 0;
}

void Attribute::visitTop(shared_ptr<Top> node) {
	for (vector<shared_ptr<Node>>::iterator itr = node->members->begin();
			itr != node->members->end(); ++itr) {
		(*itr)->accept(this);
	}
}

void Attribute::visitMethod(shared_ptr<Method> node) {
	u32string name = node->name->token->text;
	node->returnType->accept(this);
	if (node->returnType->type->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	paramTypes = shared_ptr<vector<shared_ptr<Type>>>(new vector<shared_ptr<Type>>());
	shared_ptr<MethodType> methodType = shared_ptr<MethodType>(
			new MethodType(node->returnType->type, paramTypes, node->isVarParam));
	node->type = methodType;
	if (local->lookupLocal(name)) {
		cout << node->name->token->line << " line, " << node->name->token->column
				<< " column, error: function '" << toUtf8(name) << "' redefined" << endl;
		result = 1;
		return;
	}
	local->add(name, node->type);

	shared_ptr<Scope<Type>> outScope = move(local);
	local = shared_ptr<Scope<Type>>(new Scope<Type>(outScope)); // scope for parameters
	for (shared_ptr<Node> &param : *node->params) {
		param->accept(this);
		if (result != 0) {
			node->type = ctx->errorType;
			return;
		}
	}
	paramTypes = 0;
	if (node->body) {
		node->body->accept(this);
	}
	local = outScope;
}

void Attribute::visitParameter(shared_ptr<Parameter> node) {
	node->typeNode->accept(this);
	if (node->typeNode->type->kind == ERROR_TYPE) {
		return;
	}
	paramTypes->push_back(node->typeNode->type);
	u32string name = node->name->token->text;
	if (local->lookupLocal(name)) {
		cout << node->name->token->line << " line, " << node->name->token->column
				<< " column, error: parameter '" << toUtf8(name) << "' redefined" << endl;
		result = 1;
		return;
	}
	local->add(name, node->typeNode->type);
}

void Attribute::visitArrayInitializer(shared_ptr<ArrayInitializer> node) {
	shared_ptr<Type> type;
	for (shared_ptr<Node> &exp : *node->inits) {
		exp->accept(this);
		if (!type) {
			type = exp->type;
		} else if (!type->equals(exp->type)) {
			node->type = ctx->errorType;
			Location loc = exp->getLocation();
			cout << loc.line << " line, " << loc.column
					<< " column, error: all elements in one array must have same type of the declared type"
					<< endl;
			result = 1;
			return;
		}
	}
	if (type) {
		node->type = make_shared<ArrayType>(type);
	}
}

void Attribute::visitBlock(shared_ptr<Block> node) {
	shared_ptr<Scope<Type>> outScope = move(local);
	local = shared_ptr<Scope<Type>>(new Scope<Type>(outScope));
	for (shared_ptr<Node> &stat : *node->elems) {
		stat->accept(this);
		if (result != 0) {
			return;
		}
	}
	local = outScope;
}

void Attribute::visitVarDeclStatement(shared_ptr<VarDeclStatement> node) {
	node->varDecl->accept(this);
}

void Attribute::visitExpressionStatement(shared_ptr<ExpressionStatement> node) {
	node->exp->accept(this);
}

void Attribute::visitIfStatement(shared_ptr<IfStatement> node) {
	node->condExp->accept(this);
	if (result != 0) {
		return;
	}
	node->thenStmt->accept(this);
	if (result != 0) {
		return;
	}
	if (node->elseStmt) {
		node->elseStmt->accept(this);
	}
}

void Attribute::visitWhileStatement(shared_ptr<WhileStatement> node) {
	node->condExp->accept(this);
	node->body->accept(this);
}

void Attribute::visitBreakStatement(shared_ptr<BreakStatement> node) {
}

void Attribute::visitContinueStatement(shared_ptr<ContinueStatement> node) {
}

void Attribute::visitReturnStatement(shared_ptr<ReturnStatement> node) {
	if (node->exp) {
		node->exp->accept(this);
	}
}

void Attribute::printBinaryError(shared_ptr<Term> op, shared_ptr<Type> left,
		shared_ptr<Type> right) {
	cout << op->token->line << " line, " << op->token->column
			<< " column, error: can't apply value of type of " << left->name << " and "
			<< right->name << " to binary operator " << toUtf8(op->token->text) << endl;
}

void Attribute::printUnaryError(shared_ptr<Term> op, shared_ptr<Type> expType) {
	cout << op->token->line << " line, " << op->token->column
			<< " column, error: can't apply value of type of " << expType->name
			<< " to unary operator " << toUtf8(op->token->text) << endl;
}

void Attribute::visitBinaryExpression(shared_ptr<BinaryExpression> node) {
	node->left->accept(this);
	node->right->accept(this);
	shared_ptr<Type> ltype = node->left->type;
	shared_ptr<Type> rtype = node->right->type;
	shared_ptr<Term> op = node->op;
	if (ltype->kind == ERROR_TYPE || rtype->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	if (op->token->id == OROR || op->token->id == ANDAND) {
		if (ltype->kind == BOOL_TYPE && rtype->kind == BOOL_TYPE) {
			node->type = ctx->boolType;
		} else {
			printBinaryError(op, ltype, rtype);
			node->type = ctx->errorType;
			result = 1;
		}
	} else if (op->token->id == EQEQ || op->token->id == NOTEQ) {
		if (ltype->kind == BOOL_TYPE && rtype->kind == BOOL_TYPE) {
			node->type = ctx->boolType;
		} else if (ltype->isNumer() && rtype->isNumer()) {
			node->type = ctx->boolType;
		} else {
			printBinaryError(op, ltype, rtype);
			node->type = ctx->errorType;
			result = 1;
		}
	} else if (op->token->id == LT || op->token->id == GT || op->token->id == LTEQ
			|| op->token->id == GTEQ) {
		if (ltype->isNumer() && rtype->isNumer()) {
			node->type = ctx->boolType;
		} else {
			printBinaryError(op, ltype, rtype);
			node->type = ctx->errorType;
			result = 1;
		}
	} else if (op->token->id == OR || op->token->id == XOR || op->token->id == AND
			|| op->token->id == LSHIFT || op->token->id == RSHIFT || op->token->id == MOD) {
		if (ltype->kind == INT_TYPE && rtype->kind == INT_TYPE) {
			node->type = ctx->intType;
		} else {
			printBinaryError(op, ltype, rtype);
			node->type = ctx->errorType;
			result = 1;
		}
	} else if (op->token->id == PLUS || op->token->id == MINUS || op->token->id == MULT
			|| op->token->id == DIV) {
		if (ltype->isNumer() && rtype->isNumer()) {
			if (ltype->kind == INT_TYPE && rtype->kind == INT_TYPE) {
				node->type = ctx->intType;
			} else {
				node->type = ctx->doubleType;
			}
		} else {
			printBinaryError(op, ltype, rtype);
			node->type = ctx->errorType;
			result = 1;
		}
	} else {
		printBinaryError(op, ltype, rtype);
		node->type = ctx->errorType;
		result = 1;
	}
}

void Attribute::visitUnaryExpression(shared_ptr<UnaryExpression> node) {
	node->exp->accept(this);
	shared_ptr<Type> expType = node->exp->type;
	if (expType->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	shared_ptr<Term> op = node->op;
	if (op->token->id == PLUS || op->token->id == MINUS) {
		if (expType->isNumer()) {
			node->type = expType;
		} else {
			printUnaryError(op, expType);
			node->type = ctx->errorType;
			result = 1;
		}
	} else if (op->token->id == COMP) {
		if (expType->kind == INT_TYPE) {
			node->type = expType;
		} else {
			printUnaryError(op, expType);
			node->type = ctx->errorType;
			result = 1;
		}
	} else if (op->token->id == NOT) {
		if (expType->kind == BOOL_TYPE) {
			node->type = expType;
		} else {
			printUnaryError(op, expType);
			node->type = ctx->errorType;
			result = 1;
		}
	} else {
		printUnaryError(op, expType);
		node->type = ctx->errorType;
		result = 1;
	}
}

void Attribute::visitCastExpression(shared_ptr<CastExpression> node) {
	node->exp->accept(this);
	shared_ptr<Type> expType = node->exp->type;
	if (expType->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	node->typeNode->accept(this);
	shared_ptr<Type> toType = node->typeNode->type;
	if (toType->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	if (expType->isNumer() && toType->isNumer()) {
		node->type = toType;
	} else {
		Location loc = node->getLocation();
		cout << loc.line << " line, " << loc.column << " column, error: can't cast value from type "
				<< expType->name << " to " << toType->name << endl;
		node->type = ctx->errorType;
		result = 1;
	}
}

void Attribute::visitIdentifier(shared_ptr<Identifier> node) {
	u32string name = node->name->token->text;
	shared_ptr<Type> type = local->lookup(name);
	if (type) {
		node->type = type;
	} else {
		type = local->lookup(name);
		if (type) {
			node->type = type;
		} else {
			cout << node->name->token->line << " line, " << node->name->token->column
					<< " column, error: variable '" << toUtf8(name) << "' can't be resolved"
					<< endl;
			node->type = ctx->errorType;
			result = 1;
		}
	}
}

void Attribute::visitArrayAccess(shared_ptr<ArrayAccess> node) {
	node->index->accept(this);
	if (node->index->type->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	node->array->accept(this);
	if (node->array->type->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	if (node->array->type->kind != ARRAY_TYPE) {
		Location loc = node->getLocation();
		cout << loc.line << " line, " << loc.column
				<< " column, error: can't use value of non-array as array" << endl;
		node->type = ctx->errorType;
		result = 1;
		return;
	}
	shared_ptr<ArrayType> arrType = dynamic_pointer_cast<ArrayType>(node->array->type);
	node->type = arrType->elemType;
}

void Attribute::visitMethodInvocation(shared_ptr<MethodInvocation> node) {
	for (vector<shared_ptr<Node>>::iterator itr = node->args->begin(); itr != node->args->end();
			++itr) {
		(*itr)->accept(this);
	}
	u32string name = node->name->token->text;
	shared_ptr<Type> type = local->lookup(name);
	if (!type) {
		cout << node->name->token->line << " line, " << node->name->token->column
				<< " column, error: function '" << toUtf8(name) << "' can't be resolved" << endl;
		node->type = ctx->errorType;
		result = 1;
		return;
	}

	if (type->kind != METHOD_TYPE) {
		cout << node->name->token->line << " line, " << node->name->token->column
				<< " column, error: variable '" << toUtf8(name) << "' of type" << type->name
				<< " can't be used as function" << endl;
		node->type = ctx->errorType;
		result = 1;
		return;
	}

	shared_ptr<MethodType> methodType = dynamic_pointer_cast<MethodType>(type);
	node->type = methodType->returnType;
}

void Attribute::visitAssignment(shared_ptr<Assignment> node) {
	node->left->accept(this);
	shared_ptr<Type> ltype = node->left->type;
	if (ltype->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	node->right->accept(this);
	shared_ptr<Type> rtype = node->right->type;
	if (rtype->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		return;
	}
	node->type = ltype;
}

void Attribute::visitEmptyStatement(shared_ptr<EmptyStatement> node) {
}

void Attribute::visitXcreaseStatement(shared_ptr<XcreaseStatement> node) {
	node->exp->accept(this);
	shared_ptr<Type> expType = node->exp->type;
	if (expType->kind == ERROR_TYPE) {
		node->type = ctx->errorType;
		result = 1;
		return;
	}
	if (expType->isNumer()) {
		node->type = expType;
	} else {
		Location loc = node->op->getLocation();
		cout << loc.line << " line, " << loc.column
				<< " column, error: can't apply value of type of " << expType->name
				<< " to operator " << toUtf8(node->op->token->text) << endl;
		node->type = ctx->errorType;
		result = 1;
	}
}

void Attribute::visitForStatement(shared_ptr<ForStatement> node) {
	shared_ptr<Scope<Type>> outScope = move(local);
	local = shared_ptr<Scope<Type>>(new Scope<Type>(outScope));
	if (node->init) {
		node->init->accept(this);
		if (result != 0) {
			return;
		}
	}
	if (node->condExp) {
		node->condExp->accept(this);
		if (node->condExp->type->kind == ERROR_TYPE) {
			return;
		}
	}
	if (node->update) {
		node->update->accept(this);
		if (result != 0) {
			return;
		}
	}
	if (node->body) {
		node->body->accept(this);
	}
	local = outScope;
}

void Attribute::visitDeclarator(shared_ptr<Declarator> node) {
	u32string name = node->name->token->text;
	if (local->lookupLocal(name)) {
		cout << node->name->token->line << " line, " << node->name->token->column
				<< " column, error: variable '" << toUtf8(name) << "' redefined" << endl;
		result = 1;
		return;
	}
	local->add(name, varDeclNode->typeNode->type);
	if (node->init) {
		node->init->accept(this);
		if (node->init->type && node->init->type->kind == ERROR_TYPE) {
			return;
		}
		if (!node->init->type) { // empty array
			node->init->type = varDeclNode->typeNode->type;
		}
	}
}


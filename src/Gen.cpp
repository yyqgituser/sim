#include "Gen.h"

#include <iostream>

#include "SimTokenType.h"
#include "misc.h"
#include "MethodSymbol.h"
#include "VarSymbol.h"
#include "MethodType.h"
#include "ArrayType.h"

using namespace std;
using namespace util;

Gen::Gen(string filename, shared_ptr<Context> simCtx) :
		filename(filename), simCtx(simCtx), result(0), global(new Scope<sym::Symbol>(0)), local(
				new Scope<sym::Symbol>(0)), isLocal(false), function(0), argument(0), block(0), value(
				0), reachable(false), breaked(false), isLeftValue(false), loopContBlock(0), loopEndBlock(
				0) {
	context = make_shared<LLVMContext>();
	module = make_shared<Module>(filename, *context);
	builder = make_shared<IRBuilder<>>(*context);
}

Gen::~Gen() {
}

void Gen::visitLiteral(shared_ptr<Literal> node) {
	string text;
	double dval;
	switch (node->term->token->id) {
	case INT_LITERAL:
		text = toUtf8(node->term->token->text);
		value = ConstantInt::get(*context, APInt(32, text, 10));
		break;
	case DOUBLE_LITERAL:
		text = toUtf8(node->term->token->text);
		dval = stod(text);
		value = ConstantFP::get(*context, APFloat(dval));
		break;
	case STRING_LITERAL:
		text = toUtf8(unescape(node->term->token->text));
		value = builder->CreateGlobalStringPtr(text);
		break;
	case TRUE:
		value = ConstantInt::get(*context, APInt(1, 1));
		break;
	case FALSE:
		value = ConstantInt::get(*context, APInt(1, 0));
		break;
	}
}

void Gen::visitPrimitiveTypeNode(shared_ptr<PrimitiveTypeNode> node) {
}

void Gen::visitArrayTypeNode(shared_ptr<ArrayTypeNode> node) {
}

void Gen::visitTerm(shared_ptr<Term> node) {
}

void Gen::visitVarDecl(shared_ptr<VarDecl> node) {
	u32string name32 = node->name->token->text;
	string name = toUtf8(name32);
	shared_ptr<type::Type> ltype = node->typeNode->type;
	shared_ptr<VarSymbol> var = make_shared<VarSymbol>(name32, ltype);
	if (isLocal) {
		local->add(name32, var);
		var->value = createAlloca(ltype->getRawType(context), name);
		if (node->init) {
			node->init->accept(this);
			shared_ptr<type::Type> rtype = node->init->type;
			if (ltype->equals(rtype)) {
				value = builder->CreateStore(value, var->value);
			} else { // only left double, right int
				Value *doubleValue = builder->CreateSIToFP(value, ltype->getRawType(context));
				value = builder->CreateStore(doubleValue, var->value);
			}
		}
	} else {
		global->add(name32, var);
		module->getOrInsertGlobal(name, ltype->getRawType(context));
		GlobalVariable *gv = module->getNamedGlobal(name);
		gv->setLinkage(GlobalValue::CommonLinkage);
		gv->setInitializer(ltype->getDefaultValue(context));
		var->value = gv;
		if (node->init) {
			if (ltype->kind == ARRAY_TYPE) {
				Location loc = node->init->getLocation();
				cout << loc.line << " line, " << loc.column
						<< " column, error: don't support global array initialization yet" << endl;
				result = 1;
			}
			node->init->accept(this);
			shared_ptr<type::Type> rtype = node->init->type;
			if (ltype->equals(rtype)) {
				value = builder->CreateStore(value, var->value);
			} else { // only left double, right int
				Value *doubleValue = builder->CreateSIToFP(value, ltype->getRawType(context));
				value = builder->CreateStore(doubleValue, var->value);
			}
		}
	}
}

void Gen::visitTop(shared_ptr<Top> node) {
	llvm::Type *voidType = llvm::Type::getVoidTy(*context);
	FunctionType *funType = llvm::FunctionType::get(voidType, false);
	function = Function::Create(funType, Function::InternalLinkage, "static_init", *module);
	BasicBlock *bb = BasicBlock::Create(*context, "entry", function);
	builder->SetInsertPoint(bb);

	for (vector<shared_ptr<Node>>::iterator itr = node->members->begin();
			itr != node->members->end(); ++itr) {
		(*itr)->accept(this);
		if (result != 0) {
			return;
		}
	}
	builder->CreateRetVoid();
//	module->dump();

	string targetTriple = sys::getDefaultTargetTriple();
	module->setTargetTriple(targetTriple);
	string errorMsg;
	auto target = TargetRegistry::lookupTarget(targetTriple, errorMsg);
	if (!target) {
		cout << "error: " << errorMsg << endl;
		result = 1;
		return;
	}

	string irFileName = filename.substr(0, filename.length() - 4) + ".ll";
	error_code ec;
	raw_fd_ostream llFile(irFileName, ec, sys::fs::OF_None);
	if (ec) {
		cout << "error: Could not open file: " << ec.message();
		result = 1;
		return;
	}
	module->print(llFile, 0);
	llFile.flush();
	llFile.close();

	TargetOptions opt;
	auto rm = Optional<Reloc::Model>();
	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", opt, rm);
	module->setDataLayout(targetMachine->createDataLayout());
	raw_fd_ostream objFile(filename.substr(0, filename.length() - 4) + ".o", ec, sys::fs::OF_None);
	if (ec) {
		cout << "error: Could not open file: " << ec.message();
		result = 1;
		return;
	}

	legacy::PassManager pass;
	if (targetMachine->addPassesToEmitFile(pass, objFile, nullptr, CGFT_ObjectFile)) {
		cout << "internal error: TheTargetMachine can't emit a file of this type" << endl;
		result = 1;
		return;
	}

	pass.run(*module);
	objFile.flush();
	objFile.close();
}

void Gen::visitMethod(shared_ptr<Method> node) {
	u32string name = node->name->token->text;
	shared_ptr<MethodSymbol> method = make_shared<MethodSymbol>(name, node->type);
	global->add(name, method);
	shared_ptr<MethodType> methodType = dynamic_pointer_cast<MethodType>(node->type);
	FunctionType *funType = static_cast<FunctionType*>(methodType->getRawType(context));
	Function *initFun = function;
	BasicBlock *initBlock = builder->GetInsertBlock();
	function = Function::Create(funType, Function::ExternalLinkage, toUtf8(name), *module);
	method->value = function;
	for (unsigned int i = 0; i < node->params->size(); i++) {
		shared_ptr<Parameter> param = dynamic_pointer_cast<Parameter>(node->params->at(i));
		function->getArg(i)->setName(toUtf8(param->name->token->text));
	}

	if (node->body) {
		BasicBlock *bb = BasicBlock::Create(*context, "entry", function);
		builder->SetInsertPoint(bb);
		if (toUtf8(name) == "main") {
			builder->CreateCall(initFun);
		}
		shared_ptr<Scope<sym::Symbol>> outScope = move(local);
		local = make_shared<Scope<sym::Symbol>>(outScope);
		bool old = isLocal;
		isLocal = true;
		returnType = node->returnType->type;
		reachable = true;
		for (unsigned int i = 0; i < node->params->size(); i++) {
			shared_ptr<Parameter> param = dynamic_pointer_cast<Parameter>(node->params->at(i));
			argument = function->getArg(i);
			argument->setName(toUtf8(param->name->token->text));
			param->accept(this);
			argument = 0;
		}
		node->body->accept(this);
		if (reachable) {
			if (returnType->kind == VOID_TYPE) {
				builder->CreateRetVoid();
			} else {
				Location loc = node->getLocation();
				cout << loc.line << " line, " << loc.column
						<< " column, error: some branch of function '" << toUtf8(name)
						<< "' don't have return statement" << endl;
				result = 1;
				function = initFun;
				builder->SetInsertPoint(initBlock);
				return;
			}
		}
		value = 0;
		returnType = 0;
		isLocal = old;
		local = outScope;
	}

	if (result != 0) {
		function = initFun;
		builder->SetInsertPoint(initBlock);
		return;
	}

	if (verifyFunction(*function, &errs())) {
		Location loc = node->getLocation();
		cout << loc.line << " line, " << loc.column << " column, error: verification of function '"
				<< toUtf8(name) << "' failed" << endl;
		//module->dump();
		result = 1;
	}
	function = initFun;
	builder->SetInsertPoint(initBlock);

}

void Gen::visitParameter(shared_ptr<Parameter> node) {
	u32string name = node->name->token->text;
	shared_ptr<type::Type> type = node->typeNode->type;
	shared_ptr<sym::VarSymbol> var = make_shared<sym::VarSymbol>(name, type);
	local->add(name, var);
	var->value = createAlloca(type->getRawType(context), toUtf8(name));
	builder->CreateStore(argument, var->value);
}

void Gen::visitArrayInitializer(shared_ptr<ArrayInitializer> node) {
	shared_ptr<type::ArrayType> arrType = dynamic_pointer_cast<type::ArrayType>(node->type);
	shared_ptr<type::Type> elemType = arrType->elemType;
	unsigned int size = node->inits->size();
	llvm::Type *elemRawType = elemType->getRawType(context);
	llvm::ArrayType *rawType = llvm::ArrayType::get(elemRawType, size);
	AllocaInst *arrValue = createAlloca(rawType, "array");
	Value *zero = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
	Value *indices[] = { zero, zero };
	for (unsigned int i = 0; i < node->inits->size(); i++) {
		auto exp = node->inits->at(i);
		exp->accept(this);
		indices[1] = ConstantInt::get(llvm::Type::getInt32Ty(*context), i);
		Value *ptr = builder->CreateInBoundsGEP(rawType, arrValue, indices);
		builder->CreateStore(value, ptr);
		value = 0;
	}
	indices[1] = zero;
	value = builder->CreateInBoundsGEP(rawType, arrValue, indices);
}

void Gen::visitBlock(shared_ptr<Block> node) {
	shared_ptr<Scope<sym::Symbol>> outScope = move(local);
	local = shared_ptr<Scope<sym::Symbol>>(new Scope<sym::Symbol>(outScope));
	for (auto &s : *node->elems) {
		Location loc = s->getLocation();
		if (!reachable) {
			cout << loc.line << " line, " << loc.column << " column, error: code is unreachable"
					<< endl;
			result = 1;
			break;
		}
		s->accept(this);
	}
	local = outScope;
}

void Gen::visitVarDeclStatement(shared_ptr<VarDeclStatement> node) {
	node->varDecl->accept(this);
}

void Gen::visitExpressionStatement(shared_ptr<ExpressionStatement> node) {
	node->exp->accept(this);
}

void Gen::visitIfStatement(shared_ptr<IfStatement> node) {
	node->condExp->accept(this);
	Value *cond = value;
	value = 0;
	BasicBlock *thenBlock = BasicBlock::Create(*context, "then", function);
	BasicBlock *ifcontBlock = 0;
	if (node->elseStmt) {
		BasicBlock *elseBlock = BasicBlock::Create(*context, "else");
		builder->CreateCondBr(cond, thenBlock, elseBlock);
		builder->SetInsertPoint(thenBlock);
		node->thenStmt->accept(this);
		if (reachable) {
			ifcontBlock = BasicBlock::Create(*context, "ifcont");
			builder->CreateBr(ifcontBlock);
		}
		function->getBasicBlockList().push_back(elseBlock);
		builder->SetInsertPoint(elseBlock);
		reachable = true;
		node->elseStmt->accept(this);
		if (reachable) {
			if (!ifcontBlock) {
				ifcontBlock = BasicBlock::Create(*context, "ifcont");
			}
			builder->CreateBr(ifcontBlock);
		}
	} else {
		ifcontBlock = BasicBlock::Create(*context, "ifcont");
		builder->CreateCondBr(cond, thenBlock, ifcontBlock);
		builder->SetInsertPoint(thenBlock);
		node->thenStmt->accept(this);
		if (reachable) {
			builder->CreateBr(ifcontBlock);
		}
	}
	if (ifcontBlock) {
		function->getBasicBlockList().push_back(ifcontBlock);
		builder->SetInsertPoint(ifcontBlock);
		reachable = true;
	}
}

void Gen::visitWhileStatement(shared_ptr<WhileStatement> node) {
	BasicBlock *condBlock = BasicBlock::Create(*context, "while.cond", function);
	BasicBlock *bodyBlock = BasicBlock::Create(*context, "while.body");
	BasicBlock *endBlock = BasicBlock::Create(*context, "while.end");
	builder->CreateBr(condBlock);
	builder->SetInsertPoint(condBlock);
	node->condExp->accept(this);
	Value *cond = value;
	value = 0;
	builder->CreateCondBr(cond, bodyBlock, endBlock);

	function->getBasicBlockList().push_back(bodyBlock);
	builder->SetInsertPoint(bodyBlock);
	BasicBlock *oldCont = loopContBlock;
	BasicBlock *oldEnd = loopEndBlock;
	loopContBlock = condBlock;
	loopEndBlock = endBlock;
	node->body->accept(this);
	loopContBlock = oldCont;
	loopEndBlock = oldEnd;
	builder->CreateBr(condBlock);

	function->getBasicBlockList().push_back(endBlock);
	builder->SetInsertPoint(endBlock);
}

void Gen::visitBreakStatement(shared_ptr<BreakStatement> node) {
	if (!loopEndBlock) {
		Location loc = node->getLocation();
		cout << loc.line << " line, " << loc.column
				<< " column, error: 'break' statement can't occur outside loop" << endl;
		result = 1;
		return;
	}
	builder->CreateBr(loopEndBlock);
	reachable = false;
	breaked = true;
}

void Gen::visitContinueStatement(shared_ptr<ContinueStatement> node) {
	if (!loopEndBlock) {
		Location loc = node->getLocation();
		cout << loc.line << " line, " << loc.column
				<< " column, error: 'continue' statement can't occur outside loop" << endl;
		result = 1;
		return;
	}
	builder->CreateBr(loopContBlock);
	reachable = false;
}

void Gen::visitReturnStatement(shared_ptr<ReturnStatement> node) {
	if (node->exp) {
		node->exp->accept(this);
		value = builder->CreateRet(value);
	} else {
		value = builder->CreateRetVoid();
	}
	reachable = false;
}

void Gen::visitBinaryExpression(shared_ptr<BinaryExpression> node) {
	shared_ptr<type::Type> ltype = node->left->type;
	shared_ptr<type::Type> rtype = node->right->type;
	shared_ptr<Term> op = node->op;
	node->left->accept(this);
	Value *lvalue = value;
	value = 0;
	node->right->accept(this);
	Value *rvalue = value;
	value = 0;
	if (op->token->id == OROR) { // TODO: implement shortcut
		value = builder->CreateOr(lvalue, rvalue);
	} else if (op->token->id == ANDAND) {
		value = builder->CreateAnd(lvalue, rvalue);
	} else if (op->token->id == EQEQ || op->token->id == NOTEQ || op->token->id == LT
			|| op->token->id == GT || op->token->id == LTEQ || op->token->id == GTEQ) {
		CmpInst::Predicate pred;
		if (ltype->kind == BOOL_TYPE || (ltype->kind == INT_TYPE && rtype->kind == INT_TYPE)) {
			pred = tokenTypeToCmpPredicate(false, (SimTokenType) op->token->id);
		} else {
			if (ltype->kind == INT_TYPE && rtype->kind == DOUBLE_TYPE) {
				lvalue = builder->CreateSIToFP(lvalue, rtype->getRawType(context));
			} else if (ltype->kind == DOUBLE_TYPE && rtype->kind == INT_TYPE) {
				rvalue = builder->CreateSIToFP(rvalue, ltype->getRawType(context));
			}
			pred = tokenTypeToCmpPredicate(true, (SimTokenType) op->token->id);
		}
		value = builder->CreateCmp(pred, lvalue, rvalue);
	} else if (op->token->id == OR) {
		value = builder->CreateOr(lvalue, rvalue);
	} else if (op->token->id == XOR) {
		value = builder->CreateXor(lvalue, rvalue);
	} else if (op->token->id == AND) {
		value = builder->CreateAnd(lvalue, rvalue);
	} else if (op->token->id == LSHIFT) {
		value = builder->CreateShl(lvalue, rvalue);
	} else if (op->token->id == RSHIFT) {
		value = builder->CreateAShr(lvalue, rvalue);
	} else if (op->token->id == MOD) {
		value = builder->CreateSRem(lvalue, rvalue);
	} else if (op->token->id == PLUS) {
		if (ltype->kind == INT_TYPE && rtype->kind == INT_TYPE) {
			value = builder->CreateAdd(lvalue, rvalue);
		} else {
			if (ltype->kind == INT_TYPE && rtype->kind == DOUBLE_TYPE) {
				lvalue = builder->CreateSIToFP(lvalue, rtype->getRawType(context));
			} else if (ltype->kind == DOUBLE_TYPE && rtype->kind == INT_TYPE) {
				rvalue = builder->CreateSIToFP(rvalue, ltype->getRawType(context));
			}
			value = builder->CreateFAdd(lvalue, rvalue);
		}
	} else if (op->token->id == MINUS) {
		if (ltype->kind == INT_TYPE && rtype->kind == INT_TYPE) {
			value = builder->CreateSub(lvalue, rvalue);
		} else {
			if (ltype->kind == INT_TYPE && rtype->kind == DOUBLE_TYPE) {
				lvalue = builder->CreateSIToFP(lvalue, rtype->getRawType(context));
			} else if (ltype->kind == DOUBLE_TYPE && rtype->kind == INT_TYPE) {
				rvalue = builder->CreateSIToFP(rvalue, ltype->getRawType(context));
			}
			value = builder->CreateFSub(lvalue, rvalue);
		}
	} else if (op->token->id == MULT) {
		if (ltype->kind == INT_TYPE && rtype->kind == INT_TYPE) {
			value = builder->CreateMul(lvalue, rvalue);
		} else {
			if (ltype->kind == INT_TYPE && rtype->kind == DOUBLE_TYPE) {
				lvalue = builder->CreateSIToFP(lvalue, rtype->getRawType(context));
			} else if (ltype->kind == DOUBLE_TYPE && rtype->kind == INT_TYPE) {
				rvalue = builder->CreateSIToFP(rvalue, ltype->getRawType(context));
			}
			value = builder->CreateFMul(lvalue, rvalue);
		}
	} else if (op->token->id == DIV) {
		if (ltype->kind == INT_TYPE && rtype->kind == INT_TYPE) {
			value = builder->CreateSDiv(lvalue, rvalue);
		} else {
			if (ltype->kind == INT_TYPE && rtype->kind == DOUBLE_TYPE) {
				lvalue = builder->CreateSIToFP(lvalue, rtype->getRawType(context));
			} else if (ltype->kind == DOUBLE_TYPE && rtype->kind == INT_TYPE) {
				rvalue = builder->CreateSIToFP(rvalue, ltype->getRawType(context));
			}
			value = builder->CreateFDiv(lvalue, rvalue);
		}
	}
}

void Gen::visitUnaryExpression(shared_ptr<UnaryExpression> node) {
	shared_ptr<Term> op = node->op;
	shared_ptr<type::Type> type = node->exp->type;
	node->exp->accept(this);
	if (op->token->id == MINUS) {
		if (type->kind == DOUBLE_TYPE) {
			value = builder->CreateFNeg(value);
		} else { // INT_TYPE
			Value *zero = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
			value = builder->CreateSub(zero, value);
		}
	} else if (op->token->id == COMP) {
		Value *none = ConstantInt::get(llvm::Type::getInt32Ty(*context), -1);
		value = builder->CreateXor(value, none);
	} else { // op->token->id == NOT
		Value *trueValue = ConstantInt::getTrue(*context);
		value = builder->CreateXor(value, trueValue);
	}
}

void Gen::visitCastExpression(shared_ptr<CastExpression> node) {
	shared_ptr<type::Type> fromType = node->exp->type;
	shared_ptr<type::Type> toType = node->typeNode->type;
	if (fromType->equals(toType)) {
		return;
	}
	node->exp->accept(this);
	Value *exp = value;
	value = 0;
	if (toType->kind == INT_TYPE) {
		value = builder->CreateFPToSI(exp, toType->getRawType(context));
	} else if (toType->kind == DOUBLE_TYPE) {
		value = builder->CreateSIToFP(exp, toType->getRawType(context));
	}
}

void Gen::visitIdentifier(shared_ptr<Identifier> node) {
	u32string name = node->name->token->text;
	shared_ptr<sym::VarSymbol> var = dynamic_pointer_cast<VarSymbol>(local->lookup(name));
	if (!var) {
		var = dynamic_pointer_cast<VarSymbol>(global->lookup(name));
	}
	if (isLeftValue) {
		value = var->value;
	} else {
		value = builder->CreateLoad(var->value, toUtf8(name));
	}
}

void Gen::visitArrayAccess(shared_ptr<ArrayAccess> node) {
	bool old = isLeftValue;
	isLeftValue = false;
	node->array->accept(this);
	Value *arrPtr = value;
	node->index->accept(this);
	Value *index = value;
	isLeftValue = old;

	shared_ptr<type::ArrayType> arrayType = dynamic_pointer_cast<type::ArrayType>(
			node->array->type);
	llvm::Type *elemType = arrayType->elemType->getRawType(context);
	Value *indices[] = { index };
	value = builder->CreateInBoundsGEP(elemType, arrPtr, indices);
	if (!isLeftValue) {
		value = builder->CreateLoad(elemType, value);
	}
}

void Gen::visitMethodInvocation(shared_ptr<MethodInvocation> node) {
	u32string name = node->name->token->text;
	shared_ptr<MethodSymbol> method = dynamic_pointer_cast<MethodSymbol>(global->lookup(name));
	shared_ptr<MethodType> mtype = dynamic_pointer_cast<MethodType>(method->type);
	vector<Value*> args;
	for (unsigned int i = 0; i < mtype->paramTypes->size(); i++) {
		shared_ptr<Node> argNode = node->args->at(i);
		argNode->accept(this);
		shared_ptr<type::Type> ltype = mtype->paramTypes->at(i);
		shared_ptr<type::Type> rtype = argNode->type;
		if (!ltype->equals(rtype)) { // only left double, right int
			value = builder->CreateSIToFP(value, ltype->getRawType(context));
		}
		args.push_back(value);
		value = 0;
	}
	for (unsigned int i = mtype->paramTypes->size(); i < node->args->size(); i++) {
		node->args->at(i)->accept(this);
		args.push_back(value);
		value = 0;
	}
	value = builder->CreateCall(method->value, args);
}

void Gen::visitAssignment(shared_ptr<Assignment> node) {
	bool old = isLeftValue;
	isLeftValue = true;
	node->left->accept(this);
	Value *ptr = value;
	value = 0;
	isLeftValue = old;
	node->right->accept(this);
	shared_ptr<type::Type> ltype = node->left->type;
	shared_ptr<type::Type> rtype = node->right->type;
	if (ltype->equals(rtype)) {
		builder->CreateStore(value, ptr);
	} else { // only left double, right int
		Value *doubleValue = builder->CreateSIToFP(value, ltype->getRawType(context));
		builder->CreateStore(doubleValue, ptr);
	}
}

void Gen::visitEmptyStatement(shared_ptr<EmptyStatement> node) {
}

void Gen::visitXcreaseStatement(shared_ptr<XcreaseStatement> node) {
	bool old = isLeftValue;
	isLeftValue = true;
	node->exp->accept(this);
	Value *ptr = value;
	value = 0;
	isLeftValue = old;
	shared_ptr<Term> op = node->op;
	llvm::Type *type = node->type->getRawType(context);

	if (op->token->id == PLUSPLUS) {
		if (node->type->kind == INT_TYPE) {
			Value *rvalue = builder->CreateLoad(type, ptr);
			value = builder->CreateAdd(rvalue, ConstantInt::get(type, APInt(32, 1)));
		} else { // DOUBLE_TYPE
			Value *rvalue = builder->CreateLoad(type, ptr);
			value = builder->CreateAdd(rvalue, ConstantFP::get(type, APFloat(1.0)));
		}
	} else { // MINUSMINUS
		if (node->type->kind == INT_TYPE) {
			Value *rvalue = builder->CreateLoad(type, ptr);
			value = builder->CreateAdd(rvalue, ConstantInt::get(type, APInt(32, -1)));
		} else { // DOUBLE_TYPE
			Value *rvalue = builder->CreateLoad(type, ptr);
			value = builder->CreateAdd(rvalue, ConstantFP::get(type, APFloat(-1.0)));
		}
	}
	builder->CreateStore(value, ptr);
}

void Gen::visitForStatement(shared_ptr<ForStatement> node) {
	shared_ptr<Scope<sym::Symbol>> outScope = move(local);
	// new scope for new variable in for init
	local = shared_ptr<Scope<sym::Symbol>>(new Scope<sym::Symbol>(outScope));
	if (node->init) {
		node->init->accept(this);
	}
	if (node->condExp) {
		BasicBlock *condBlock = BasicBlock::Create(*context, "for.cond", function);
		BasicBlock *bodyBlock = BasicBlock::Create(*context, "for.body");
		BasicBlock *endBlock = BasicBlock::Create(*context, "for.end");
		BasicBlock *updateBlock = 0;
		BasicBlock *contBlock;
		if (node->update) {
			contBlock = updateBlock = BasicBlock::Create(*context, "for.update");
		} else {
			contBlock = condBlock;
		}

		builder->CreateBr(condBlock);
		builder->SetInsertPoint(condBlock);
		node->condExp->accept(this);
		Value *cond = value;
		value = 0;
		builder->CreateCondBr(cond, bodyBlock, endBlock);

		function->getBasicBlockList().push_back(bodyBlock);
		builder->SetInsertPoint(bodyBlock);
		BasicBlock *oldCont = loopContBlock;
		BasicBlock *oldEnd = loopEndBlock;
		loopContBlock = contBlock;
		loopEndBlock = endBlock;
		node->body->accept(this);
		loopContBlock = oldCont;
		loopEndBlock = oldEnd;
		builder->CreateBr(contBlock);

		if (node->update) {
			function->getBasicBlockList().push_back(updateBlock);
			builder->SetInsertPoint(updateBlock);
			node->update->accept(this);
			builder->CreateBr(condBlock);
		}

		function->getBasicBlockList().push_back(endBlock);
		builder->SetInsertPoint(endBlock);
	} else {
		BasicBlock *bodyBlock = BasicBlock::Create(*context, "for.body", function);
		BasicBlock *endBlock = BasicBlock::Create(*context, "for.end");
		BasicBlock *updateBlock = 0;
		BasicBlock *contBlock;
		if (node->update) {
			contBlock = updateBlock = BasicBlock::Create(*context, "for.update");
		} else {
			contBlock = bodyBlock;
		}

		builder->CreateBr(bodyBlock);
		builder->SetInsertPoint(bodyBlock);
		BasicBlock *oldCont = loopContBlock;
		BasicBlock *oldEnd = loopEndBlock;
		bool oldBreaked = breaked;
		loopContBlock = contBlock;
		loopEndBlock = endBlock;
		breaked = false;
		node->body->accept(this);
		loopContBlock = oldCont;
		loopEndBlock = oldEnd;
		builder->CreateBr(contBlock);

		if (node->update) {
			function->getBasicBlockList().push_back(updateBlock);
			builder->SetInsertPoint(updateBlock);
			node->update->accept(this);
			builder->CreateBr(bodyBlock);
		}

		if (breaked) {
			function->getBasicBlockList().push_back(endBlock);
			builder->SetInsertPoint(endBlock);
		} else {
			reachable = false;
		}
		breaked = oldBreaked;
	}
	local = outScope;
}

// TODO: remove createAlloca by a two pass visit
AllocaInst* Gen::createAlloca(llvm::Type *type, StringRef VarName) {
	IRBuilder<> tmp(&function->getEntryBlock(), function->getEntryBlock().begin());
	return tmp.CreateAlloca(type, nullptr, VarName);
}

CmpInst::Predicate Gen::tokenTypeToCmpPredicate(bool isFloat, SimTokenType id) {
	if (isFloat) {
		switch (id) {
		case EQEQ:
			return CmpInst::FCMP_OEQ;
		case NOTEQ:
			return CmpInst::FCMP_ONE;
		case LT:
			return CmpInst::FCMP_OLT;
		case GT:
			return CmpInst::FCMP_OGT;
		case LTEQ:
			return CmpInst::FCMP_OLE;
		case GTEQ:
			return CmpInst::FCMP_OGE;
		default:
			return CmpInst::BAD_ICMP_PREDICATE;
		}
	} else {
		switch (id) {
		case EQEQ:
			return CmpInst::ICMP_EQ;
		case NOTEQ:
			return CmpInst::ICMP_NE;
		case LT:
			return CmpInst::ICMP_SLT;
		case GT:
			return CmpInst::ICMP_SGT;
		case LTEQ:
			return CmpInst::ICMP_SLE;
		case GTEQ:
			return CmpInst::ICMP_SGE;
		default:
			return CmpInst::BAD_ICMP_PREDICATE;
		}
	}
}

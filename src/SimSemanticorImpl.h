#ifndef _SIMSEMANTICORIMPL_H
#define _SIMEMANTICORIMPL_H

#include <string>
#include <cmath>
#include <codecvt>
#include <locale>
#include <stack>
#include <vector>

#include "SimSemanticor.h"
#include "SimToken.h"
#include "Node.h"
#include "Literal.h"
#include "Top.h"

using namespace std;
using namespace parser;
using namespace node;

class SimSemanticorImpl: public SimSemanticor {
public:
	SimSemanticorImpl();

	virtual ~SimSemanticorImpl();

	virtual shared_ptr<Node> getAst();

	virtual void outToken(SimToken *token);

	virtual bool declLiteral();

	virtual bool declTop();

	virtual bool declTopVar();

	virtual bool declVarDecl();

	virtual bool declFirstDeclarator();

	virtual bool declNextDeclarator();

	virtual bool declVarWithoutInit();

	virtual bool declVarWithInit();

	virtual bool declMethodWithoutBody();

	virtual bool declMethodWithBody();

	virtual bool declEmptyParams();

	virtual bool firstParam();

	virtual bool nextParam();

	virtual bool declVarParam();

	virtual bool declVarParam1();

	virtual bool declParam();

	virtual bool declEmptyArrayInitializer();

	virtual bool declArrayInitializer();

	virtual bool firstVariableInitializer();

	virtual bool nextVariableInitializer();

	virtual bool declEmptyBlock();

	virtual bool declBlock();

	virtual bool firstBlockStatement();

	virtual bool nextBlockStatement();

	virtual bool declLocalVar();

	virtual bool declEmptyStatement();

	virtual bool declExpressionStatement();

	virtual bool declAssignment();

	virtual bool declXcreaseStatement();

	virtual bool declIfStatement();

	virtual bool declIfElseStatement();

	virtual bool declWhileStatement();

	virtual bool declBreakStatement();

	virtual bool declContinueStatement();

	virtual bool declReturnWithoutExpression();

	virtual bool declReturnWithExpression();

	virtual bool declBinaryExpr();

	virtual bool declPrefixExpression();

	virtual bool declCastExpression();

	virtual bool declParanExpression();

	virtual bool declIdentifier();

	virtual bool declArrayAccess();

	virtual bool declMethodInvocation();

	virtual bool declEmptyArgumentList();

	virtual bool firstArgument();

	virtual bool nextArgument();

	virtual bool declArrayType();

	virtual bool declPrimitiveType();

	virtual bool declForStatement();

	virtual bool declEmptyForInit();

	virtual bool declEmptyForCondition();

	virtual bool declEmptyForUpdate();

private:
	stack<shared_ptr<Term>> terms;

	void push(shared_ptr<Term> node);

	shared_ptr<Term> pop();

	stack<shared_ptr<Node>> nodes;

	void pushNode(shared_ptr<Node> node);

	shared_ptr<Node> popNode();

	stack<shared_ptr<vector<shared_ptr<Node>>>> lists;

	void pushList(shared_ptr<vector<shared_ptr<Node>>> list);

	shared_ptr<vector<shared_ptr<Node>>> popList();

	shared_ptr<Top> top;

	shared_ptr<vector<shared_ptr<Node>>> members;

	bool isVarParam;
};

#endif


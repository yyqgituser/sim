#ifndef _SIMSEMANTICOR_H
#define _SIMSEMANTICOR_H

#include "SimToken.h"

namespace parser {

class SimSemanticor {
public:
  virtual ~SimSemanticor() {};

  // This function take the ownership of token
  virtual void outToken(SimToken* token) = 0;

  virtual bool declTop() = 0;

  virtual bool declTopVar() = 0;

  virtual bool declVarWithoutInit() = 0;

  virtual bool declVarWithInit() = 0;

  virtual bool declMethodWithoutBody() = 0;

  virtual bool declMethodWithBody() = 0;

  virtual bool declEmptyParams() = 0;

  virtual bool declVarParam() = 0;

  virtual bool declVarParam1() = 0;

  virtual bool firstParam() = 0;

  virtual bool nextParam() = 0;

  virtual bool declParam() = 0;

  virtual bool declEmptyArrayInitializer() = 0;

  virtual bool declArrayInitializer() = 0;

  virtual bool firstVariableInitializer() = 0;

  virtual bool nextVariableInitializer() = 0;

  virtual bool declEmptyBlock() = 0;

  virtual bool declBlock() = 0;

  virtual bool firstBlockStatement() = 0;

  virtual bool nextBlockStatement() = 0;

  virtual bool declLocalVar() = 0;

  virtual bool declEmptyStatement() = 0;

  virtual bool declExpressionStatement() = 0;

  virtual bool declIfStatement() = 0;

  virtual bool declIfElseStatement() = 0;

  virtual bool declWhileStatement() = 0;

  virtual bool declForStatement() = 0;

  virtual bool declEmptyForInit() = 0;

  virtual bool declEmptyForCondition() = 0;

  virtual bool declEmptyForUpdate() = 0;

  virtual bool declBreakStatement() = 0;

  virtual bool declContinueStatement() = 0;

  virtual bool declReturnWithoutExpression() = 0;

  virtual bool declReturnWithExpression() = 0;

  virtual bool declBinaryExpr() = 0;

  virtual bool declPrefixExpression() = 0;

  virtual bool declCastExpression() = 0;

  virtual bool declParanExpression() = 0;

  virtual bool declIdentifier() = 0;

  virtual bool declArrayAccess() = 0;

  virtual bool declMethodInvocation() = 0;

  virtual bool declEmptyArgumentList() = 0;

  virtual bool firstArgument() = 0;

  virtual bool nextArgument() = 0;

  virtual bool declAssignment() = 0;

  virtual bool declXcreaseStatement() = 0;

  virtual bool declLiteral() = 0;

  virtual bool declArrayType() = 0;

  virtual bool declPrimitiveType() = 0;
};

}

#endif

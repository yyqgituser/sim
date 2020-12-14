#ifndef _SIMSEMANTICORADAPTER_H
#define _SIMSEMANTICORADAPTER_H

#include "SimSemanticor.h"
#include "SimToken.h"

namespace parser {

class SimSemanticorAdapter : public SimSemanticor {
public:
  virtual ~SimSemanticorAdapter() {};

  void outToken(SimToken* token) {
    delete token;
  }

  bool declTop() {
    //std::cout << "declTop" << std::endl;
    return true;
  }

  bool declTopVar() {
    //std::cout << "declTopVar" << std::endl;
    return true;
  }

  bool declVarWithoutInit() {
    //std::cout << "declVarWithoutInit" << std::endl;
    return true;
  }

  bool declVarWithInit() {
    //std::cout << "declVarWithInit" << std::endl;
    return true;
  }

  bool declMethodWithoutBody() {
    //std::cout << "declMethodWithoutBody" << std::endl;
    return true;
  }

  bool declMethodWithBody() {
    //std::cout << "declMethodWithBody" << std::endl;
    return true;
  }

  bool declEmptyParams() {
    //std::cout << "declEmptyParams" << std::endl;
    return true;
  }

  bool declVarParam() {
    //std::cout << "declVarParam" << std::endl;
    return true;
  }

  bool declVarParam1() {
    //std::cout << "declVarParam1" << std::endl;
    return true;
  }

  bool firstParam() {
    //std::cout << "firstParam" << std::endl;
    return true;
  }

  bool nextParam() {
    //std::cout << "nextParam" << std::endl;
    return true;
  }

  bool declParam() {
    //std::cout << "declParam" << std::endl;
    return true;
  }

  bool declEmptyArrayInitializer() {
    //std::cout << "declEmptyArrayInitializer" << std::endl;
    return true;
  }

  bool declArrayInitializer() {
    //std::cout << "declArrayInitializer" << std::endl;
    return true;
  }

  bool firstVariableInitializer() {
    //std::cout << "firstVariableInitializer" << std::endl;
    return true;
  }

  bool nextVariableInitializer() {
    //std::cout << "nextVariableInitializer" << std::endl;
    return true;
  }

  bool declEmptyBlock() {
    //std::cout << "declEmptyBlock" << std::endl;
    return true;
  }

  bool declBlock() {
    //std::cout << "declBlock" << std::endl;
    return true;
  }

  bool firstBlockStatement() {
    //std::cout << "firstBlockStatement" << std::endl;
    return true;
  }

  bool nextBlockStatement() {
    //std::cout << "nextBlockStatement" << std::endl;
    return true;
  }

  bool declLocalVar() {
    //std::cout << "declLocalVar" << std::endl;
    return true;
  }

  bool declEmptyStatement() {
    //std::cout << "declEmptyStatement" << std::endl;
    return true;
  }

  bool declExpressionStatement() {
    //std::cout << "declExpressionStatement" << std::endl;
    return true;
  }

  bool declIfStatement() {
    //std::cout << "declIfStatement" << std::endl;
    return true;
  }

  bool declIfElseStatement() {
    //std::cout << "declIfElseStatement" << std::endl;
    return true;
  }

  bool declWhileStatement() {
    //std::cout << "declWhileStatement" << std::endl;
    return true;
  }

  bool declForStatement() {
    //std::cout << "declForStatement" << std::endl;
    return true;
  }

  bool declEmptyForInit() {
    //std::cout << "declEmptyForInit" << std::endl;
    return true;
  }

  bool declEmptyForCondition() {
    //std::cout << "declEmptyForCondition" << std::endl;
    return true;
  }

  bool declEmptyForUpdate() {
    //std::cout << "declEmptyForUpdate" << std::endl;
    return true;
  }

  bool declBreakStatement() {
    //std::cout << "declBreakStatement" << std::endl;
    return true;
  }

  bool declContinueStatement() {
    //std::cout << "declContinueStatement" << std::endl;
    return true;
  }

  bool declReturnWithoutExpression() {
    //std::cout << "declReturnWithoutExpression" << std::endl;
    return true;
  }

  bool declReturnWithExpression() {
    //std::cout << "declReturnWithExpression" << std::endl;
    return true;
  }

  bool declBinaryExpr() {
    //std::cout << "declBinaryExpr" << std::endl;
    return true;
  }

  bool declPrefixExpression() {
    //std::cout << "declPrefixExpression" << std::endl;
    return true;
  }

  bool declCastExpression() {
    //std::cout << "declCastExpression" << std::endl;
    return true;
  }

  bool declParanExpression() {
    //std::cout << "declParanExpression" << std::endl;
    return true;
  }

  bool declIdentifier() {
    //std::cout << "declIdentifier" << std::endl;
    return true;
  }

  bool declArrayAccess() {
    //std::cout << "declArrayAccess" << std::endl;
    return true;
  }

  bool declMethodInvocation() {
    //std::cout << "declMethodInvocation" << std::endl;
    return true;
  }

  bool declEmptyArgumentList() {
    //std::cout << "declEmptyArgumentList" << std::endl;
    return true;
  }

  bool firstArgument() {
    //std::cout << "firstArgument" << std::endl;
    return true;
  }

  bool nextArgument() {
    //std::cout << "nextArgument" << std::endl;
    return true;
  }

  bool declAssignment() {
    //std::cout << "declAssignment" << std::endl;
    return true;
  }

  bool declXcreaseStatement() {
    //std::cout << "declXcreaseStatement" << std::endl;
    return true;
  }

  bool declLiteral() {
    //std::cout << "declLiteral" << std::endl;
    return true;
  }

  bool declArrayType() {
    //std::cout << "declArrayType" << std::endl;
    return true;
  }

  bool declPrimitiveType() {
    //std::cout << "declPrimitiveType" << std::endl;
    return true;
  }
};

}

#endif

#ifndef _SIMACTIONID_H
#define _SIMACTIONID_H

namespace parser {

enum SimActionId {
  declTop = 51,
  declTopVar = 52,
  declVarWithoutInit = 53,
  declVarWithInit = 54,
  declMethodWithoutBody = 55,
  declMethodWithBody = 56,
  declEmptyParams = 57,
  declVarParam = 58,
  declVarParam1 = 59,
  firstParam = 60,
  nextParam = 61,
  declParam = 62,
  declEmptyArrayInitializer = 63,
  declArrayInitializer = 64,
  firstVariableInitializer = 65,
  nextVariableInitializer = 66,
  declEmptyBlock = 67,
  declBlock = 68,
  firstBlockStatement = 69,
  nextBlockStatement = 70,
  declLocalVar = 71,
  declEmptyStatement = 72,
  declExpressionStatement = 73,
  declIfStatement = 74,
  declIfElseStatement = 75,
  declWhileStatement = 76,
  declForStatement = 77,
  declEmptyForInit = 78,
  declEmptyForCondition = 79,
  declEmptyForUpdate = 80,
  declBreakStatement = 81,
  declContinueStatement = 82,
  declReturnWithoutExpression = 83,
  declReturnWithExpression = 84,
  declBinaryExpr = 85,
  declPrefixExpression = 86,
  declCastExpression = 87,
  declParanExpression = 88,
  declIdentifier = 89,
  declArrayAccess = 90,
  declMethodInvocation = 91,
  declEmptyArgumentList = 92,
  firstArgument = 93,
  nextArgument = 94,
  declAssignment = 95,
  declXcreaseStatement = 96,
  declLiteral = 97,
  declArrayType = 98,
  declPrimitiveType = 99
};

}

#endif

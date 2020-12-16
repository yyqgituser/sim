#ifndef _SIMACTIONID_H
#define _SIMACTIONID_H

namespace parser {

enum SimActionId {
  declTop = 51,
  declTopVar = 52,
  declVarDecl = 53,
  declFirstDeclarator = 54,
  declNextDeclarator = 55,
  declVarWithoutInit = 56,
  declVarWithInit = 57,
  declMethodWithoutBody = 58,
  declMethodWithBody = 59,
  declEmptyParams = 60,
  declVarParam = 61,
  declVarParam1 = 62,
  firstParam = 63,
  nextParam = 64,
  declParam = 65,
  declEmptyArrayInitializer = 66,
  declArrayInitializer = 67,
  firstVariableInitializer = 68,
  nextVariableInitializer = 69,
  declEmptyBlock = 70,
  declBlock = 71,
  firstBlockStatement = 72,
  nextBlockStatement = 73,
  declLocalVar = 74,
  declEmptyStatement = 75,
  declExpressionStatement = 76,
  declIfStatement = 77,
  declIfElseStatement = 78,
  declWhileStatement = 79,
  declForStatement = 80,
  declEmptyForInit = 81,
  declEmptyForCondition = 82,
  declEmptyForUpdate = 83,
  declBreakStatement = 84,
  declContinueStatement = 85,
  declReturnWithoutExpression = 86,
  declReturnWithExpression = 87,
  declBinaryExpr = 88,
  declPrefixExpression = 89,
  declCastExpression = 90,
  declParanExpression = 91,
  declIdentifier = 92,
  declArrayAccess = 93,
  declMethodInvocation = 94,
  declEmptyArgumentList = 95,
  firstArgument = 96,
  nextArgument = 97,
  declAssignment = 98,
  declXcreaseStatement = 99,
  declLiteral = 100,
  declArrayType = 101,
  declPrimitiveType = 102
};

}

#endif

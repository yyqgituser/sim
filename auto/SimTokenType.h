#ifndef _SIMTOKENTYPE_H
#define _SIMTOKENTYPE_H

namespace parser {

enum SimTokenType {
  EOI = 0,
  SEMI = 1,
  IDENTIFIER = 2,
  EQ = 3,
  LPAREN = 4,
  RPAREN = 5,
  ELLIPSIS = 6,
  COMMA = 7,
  LBRACE = 8,
  RBRACE = 9,
  IF = 10,
  ELSE = 11,
  WHILE = 12,
  FOR = 13,
  BREAK = 14,
  CONTINUE = 15,
  RETURN = 16,
  OROR = 17,
  ANDAND = 18,
  EQEQ = 19,
  NOTEQ = 20,
  LT = 21,
  GT = 22,
  LTEQ = 23,
  GTEQ = 24,
  OR = 25,
  XOR = 26,
  AND = 27,
  LSHIFT = 28,
  RSHIFT = 29,
  PLUS = 30,
  MINUS = 31,
  MULT = 32,
  DIV = 33,
  MOD = 34,
  COMP = 35,
  NOT = 36,
  LBRACK = 37,
  RBRACK = 38,
  PLUSPLUS = 39,
  MINUSMINUS = 40,
  INT_LITERAL = 41,
  DOUBLE_LITERAL = 42,
  STRING_LITERAL = 43,
  TRUE = 44,
  FALSE = 45,
  INT = 46,
  DOUBLE = 47,
  STRING = 48,
  BOOLEAN = 49,
  VOID = 50
};

}

#endif

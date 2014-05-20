#include "lexer.hpp"
#include <stdio.h>

using namespace std;

namespace lexer {

  // the core indent / unindent
  // Token* T_INDENT = new Token("indent");
  // Token* T_UNINDENT = new Token("unindent");

  // keywords
  Keyword* T_ELSE = new Keyword("else");
  Keyword* T_IF = new Keyword("if");
  Keyword* T_IS = new Keyword("is");
  Keyword* T_RETURN = new Keyword("return");

  // constants
  Keyword* T_TRUE = new Keyword("true");
  Keyword* T_FALSE = new Keyword("false");

  // operators
  Operator* T_LPAREN = new Operator("(");
  Operator* T_RPAREN = new Operator(")");
  Operator* T_COMMA = new Operator(",");
  Operator* T_DOT = new Operator(".");
  Operator* T_PLUS = new Operator("+");
  Operator* T_MINUS = new Operator("-");
  Operator* T_MUL = new Operator("*");
  Operator* T_DIV = new Operator("/");
  Operator* T_DECLARE = new Operator(":=");
  Operator* T_ASSIGN = new Operator("=");
  Operator* T_COMPARE_EQUAL = new Operator("==");
  Operator* T_COMPARE_NOT_EQUAL = new Operator("!=");
  Operator* T_COMPARE_LEQ = new Operator("<=");
  Operator* T_COMPARE_GEQ = new Operator(">=");
  Operator* T_COMPARE_LET = new Operator("<");
  Operator* T_COMPARE_GET = new Operator(">");
  Operator* T_COLON = new Operator(":");

  KeywordVector keywordList {
      T_ELSE,
      T_IF,
      T_IS,
      T_RETURN,
      T_FALSE,
      T_TRUE
  };

  OperatorPairVector operatorPairs {
    T_LPAREN->getOperatorPair(),
    T_RPAREN->getOperatorPair(),
    T_COMMA->getOperatorPair(),
    T_DOT->getOperatorPair(),
    T_PLUS->getOperatorPair(),
    T_MINUS->getOperatorPair(),
    T_MUL->getOperatorPair(),
    T_DIV->getOperatorPair(),
    T_DECLARE->getOperatorPair(),
    T_ASSIGN->getOperatorPair(),
    T_COMPARE_EQUAL->getOperatorPair(),
    T_COMPARE_NOT_EQUAL->getOperatorPair(),
    T_COMPARE_LEQ->getOperatorPair(),
    T_COMPARE_GEQ->getOperatorPair(),
    T_COMPARE_LET->getOperatorPair(),
    T_COMPARE_GET->getOperatorPair(),
    T_COLON->getOperatorPair()
  };

  OperatorFSM operatorFSM =
    OperatorFSM(' ', NULL).addChildren(operatorPairs);


  bool isAlpha(char c) {
    if ('a' <= c && c <= 'z') {
      return true;
    } else if ('A' <= c && c <= 'Z') {
      return true;
    }
    return false;
  }

  bool isNumeric(char c) {
    return ('0' <= c && c <= '9');
  }

  bool isAlphaNumeric(char c) {
    return isAlpha(c) || isNumeric(c);
  }
}

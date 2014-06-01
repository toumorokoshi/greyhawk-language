#include "tokens.hpp"

namespace lexer {

  // the actual tokens
  const Token T_INDENT("indent");
  const Token T_UNINDENT("unindent");

  // keywords
  const Keyword T_ELSE("else");
  const Keyword T_IF("if");
  const Keyword T_IS("is");
  const Keyword T_RETURN("return");

  // constants
  const Keyword T_TRUE("true");
  const Keyword T_FALSE("false");

  // operators
  const Operator T_LPAREN("(");
  const Operator T_RPAREN(")");
  const Operator T_COMMA(",");
  const Operator T_DOT(".");
  const Operator T_PLUS("+");
  const Operator T_MINUS("-");
  const Operator T_MUL("*");
  const Operator T_DIV("/");
  const Operator T_DECLARE(":=");
  const Operator T_ASSIGN("=");
  const Operator T_COMPARE_EQUAL("==");
  const Operator T_COMPARE_NOT_EQUAL("!=");
  const Operator T_COMPARE_LEQ("<=");
  const Operator T_COMPARE_GEQ(">=");
  const Operator T_COMPARE_LET("<");
  const Operator T_COMPARE_GET(">");
  const Operator T_COLON(":");




  // Grouping the tokens up
  const KeywordVector keywordList {
      &T_ELSE,
      &T_IF,
      &T_IS,
      &T_RETURN,
      &T_FALSE,
      &T_TRUE
  };

  const OperatorPairVector operatorPairs {
    T_LPAREN.getOperatorPair(),
    T_RPAREN.getOperatorPair(),
    T_COMMA.getOperatorPair(),
    T_DOT.getOperatorPair(),
    T_PLUS.getOperatorPair(),
    T_MINUS.getOperatorPair(),
    T_MUL.getOperatorPair(),
    T_DIV.getOperatorPair(),
    T_DECLARE.getOperatorPair(),
    T_ASSIGN.getOperatorPair(),
    T_COMPARE_EQUAL.getOperatorPair(),
    T_COMPARE_NOT_EQUAL.getOperatorPair(),
    T_COMPARE_LEQ.getOperatorPair(),
    T_COMPARE_GEQ.getOperatorPair(),
    T_COMPARE_LET.getOperatorPair(),
    T_COMPARE_GET.getOperatorPair(),
    T_COLON.getOperatorPair()
  };

}

#include "tokens.hpp"

namespace lexer {

  // the actual tokens
  const Token T_INDENT("indent");
  const Token T_UNINDENT("unindent");
  const Token T_LPAREN("(");
  const Token T_RPAREN(")");

  // keywords
  const Keyword T_ELSE("else");
  const Keyword T_IF("if");
  const Keyword T_RETURN("return");

  // constants
  const Keyword T_TRUE("true");
  const Keyword T_FALSE("false");

  // operators
  const Operator T_COMMA(",", OPERATOR_CODES::COMMA);
  const Operator T_DOT(".", OPERATOR_CODES::DOT);
  const Operator T_PLUS("+", OPERATOR_CODES::PLUS);
  const Operator T_MINUS("-", OPERATOR_CODES::MINUS);
  const Operator T_MUL("*", OPERATOR_CODES::MUL);
  const Operator T_DIV("/", OPERATOR_CODES::DIV);
  const Operator T_DECLARE(":=", OPERATOR_CODES::DECLARE);
  const Operator T_ASSIGN("=", OPERATOR_CODES::ASSIGN);
  const Operator T_COMPARE_EQUAL("==", OPERATOR_CODES::COMPARE_EQUAL);
  const Operator T_COMPARE_NOT_EQUAL("!=", OPERATOR_CODES::COMPARE_NOT_EQUAL);
  const Operator T_COMPARE_LEQ("<=", OPERATOR_CODES::COMPARE_LEQ);
  const Operator T_COMPARE_GEQ(">=", OPERATOR_CODES::COMPARE_GEQ);
  const Operator T_COMPARE_LET("<", OPERATOR_CODES::COMPARE_LET);
  const Operator T_COMPARE_GET(">", OPERATOR_CODES::COMPARE_GET);
  const Operator T_COLON(":", OPERATOR_CODES::COLON);
  const Operator T_IS("is", OPERATOR_CODES::IS);



  // Grouping the tokens up
  const KeywordVector keywordList {
      &T_ELSE,
      &T_IF,
      &T_RETURN,
      &T_FALSE,
      &T_TRUE
  };

  const OperatorPairVector operatorPairs {
    *(new OperatorPair("(", &T_LPAREN)),
    *(new OperatorPair(")", &T_RPAREN)),
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
    T_COLON.getOperatorPair(),
    T_IS.getOperatorPair()
  };

}

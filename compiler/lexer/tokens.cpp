#include "tokens.hpp"

namespace lexer {

  std::map<L, std::string> tokenMap = {
    {BAD_TOKEN, "bad token"},
    {IF, "if"},
    {ELSE, "else"},
    {RETURN, "return"},
    {TRUE, "true"},
    {FALSE, "false"},
    {INDENT, "indent"},
    {UNINDENT, "unindent"},
    {LPAREN, "("},
    {RPAREN, ")"},
    {COMMA, ","},
    {DOT, "."},
    {PLUS, "+"},
    {MINUS, "-"},
    {MUL, "*"},
    {DIV, "/"},
    {DECLARE, ":="},
    {ASSIGN, "="},
    {EQUAL, "=="},
    {NOT_EQUAL, "!="},
    {LESS_OR_EQUAL, "<="},
    {GREATER_OR_EQUAL, ">="},
    {LESS_THAN, "<"},
    {GREATER_THAN, ">"},
    {COLON, ":"},
    {IS, "is"}
  };

  KeywordPair& pairFromType(L type) {
    return *new KeywordPair(tokenMap[type], type);
  }

  // Grouping the tokens up
  const KeywordPairVector keywordList {
    pairFromType(ELSE),
    pairFromType(IF),
    pairFromType(RETURN),
    pairFromType(FALSE),
    pairFromType(TRUE),
  };

  const KeywordPairVector operatorPairs {
    // todo: separate these out from binary operators
    pairFromType(LPAREN),
    pairFromType(RPAREN),
    pairFromType(COLON),
    pairFromType(COMMA),
    pairFromType(DOT),
    // binary operators
    pairFromType(PLUS),
    pairFromType(MINUS),
    pairFromType(DIV),
    pairFromType(DECLARE),
    pairFromType(ASSIGN),
    pairFromType(EQUAL),
    pairFromType(NOT_EQUAL),
    pairFromType(LESS_OR_EQUAL),
    pairFromType(GREATER_OR_EQUAL),
    pairFromType(LESS_THAN),
    pairFromType(GREATER_THAN),
    pairFromType(IS),
 };

}

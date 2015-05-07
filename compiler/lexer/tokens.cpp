#include "tokens.hpp"

namespace lexer {

  std::map<L, std::string> tokenMap = {
    {BAD_TOKEN, "bad token"},
    {CHAR, "char"},
    {INT, "integer"},
    {DOUBLE, "double"},
    {STRING, "string"},
    {IDENTIFIER, "identifier"},
    {TYPE, "type"},
    {IF, "if"},
    {ELSE, "else"},
    {FOR, "for"},
    {IN, "in"},
    {RETURN, "return"},
    {TRUE, "true"},
    {FALSE, "false"},
    {CLASS, "class"},
    {INDENT, "indent"},
    {UNINDENT, "unindent"},
    {LPAREN, "("},
    {RPAREN, ")"},
    {L_BRACKET, "["},
    {R_BRACKET, "]"},
    {L_CURLY, "{"},
    {R_CURLY, "}"},
    {SEMICOLON, ";"},
    {COMMA, ","},
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
    {DOT, "."},
    {IS, "is"},
    {INCREMENT, "+="},
    {DECREMENT, "-="},
    {WHILE, "while"}
  };

  KeywordPair& pairFromType(L type) {
    return *new KeywordPair(tokenMap[type], type);
  }

  // Grouping the tokens up
  const KeywordPairVector keywordList {
    pairFromType(ELSE),
    pairFromType(IF),
    pairFromType(FOR),
    pairFromType(IN),
    pairFromType(RETURN),
    pairFromType(FALSE),
    pairFromType(TRUE),
    pairFromType(CLASS),
    pairFromType(WHILE),
  };

  const KeywordPairVector operatorPairs {
    // todo: separate these out from binary operators
    pairFromType(LPAREN),
    pairFromType(RPAREN),
    pairFromType(L_BRACKET),
    pairFromType(R_BRACKET),
    pairFromType(L_CURLY),
    pairFromType(R_CURLY),
    pairFromType(COLON),
    pairFromType(COMMA),
    pairFromType(DOT),
    pairFromType(SEMICOLON),
    // binary operators
    pairFromType(PLUS),
    pairFromType(MINUS),
    pairFromType(MUL),
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
    pairFromType(INCREMENT),
    pairFromType(DECREMENT),
 };

}

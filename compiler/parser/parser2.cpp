#include "./parser.hpp"
#include <stdio.h>

using namespace lexer;

namespace parser {

  Node* parseProgram(TokenVector& tokens) {
    NBlock* program = new NBlock();
    TokenVector::iterator token_position = tokens.begin();
    while (token_position != tokens.end()) {
      //program->statements.push_back(parseStatement(token_position,
      //                                            tokens.end()));
    }
  }

  Node* parseFunctionDeclaration(TokenVector::iterator token_position,
                                 TokenVector::iterator token_end) {
  }

  NBlock* parseBlock(TokenVector::iterator& token_position,
                     TokenVector::iterator token_end) {
    NBlock* block = new NBlock();
    while(token_position != token_end) {
      NStatement* statement = parseStatement(token_position,
                                             token_end);
      block->statements.push_back(statement);
    }
    return block;
  }

  NStatement* parseStatement(TokenVector::iterator& token_position,
                             TokenVector::iterator token_end) {
    if (*token_position == &T_RETURN) {
      token_position++;
      return new NReturn(*parseExpression(token_position,
                                          token_end));
    } else {
      throw ParserException("Looking for statement, unable to find one");
    }
  }

  bool isNumeric(const Token& token) {
    return typeid(token) == typeid(Integer) || typeid(token) == typeid(Double);
  }

  NExpression* parseExpression(TokenVector::iterator& token_position,
                               TokenVector::iterator token_end) {

    if (*token_position == &T_TRUE) {
      token_position++;
      return new NBoolean(true);

    } else if (*token_position == &T_FALSE) {
      token_position++;
      return new NBoolean(false);

    } else if (isNumeric(**token_position)) {
      return parseNumeric(token_position, token_end);

    } else {
      throw ParserException("Looking for expression, unable to find one");
    }
  }

  NExpression* parseNumeric(TokenVector::iterator& token_position,
                            TokenVector::iterator token_end) {
    NExpression* lhs = NULL;
    if (typeid(**token_position) == typeid(Integer)) {
      auto integer = (Integer*) *token_position;
      lhs = new NInteger(integer->value);
    } else {
      throw ParserException("expected a numeric!");
    }
    return lhs;
  }
}

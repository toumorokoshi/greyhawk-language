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

  bool isBinaryOperator(const Token& token) {
    std::cout << typeid(token).name() << std::endl;
    return typeid(token) == typeid(Operator);
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
    NExpression* lhs = parseSingleNumeric(token_position,
                                          token_end);
    while (token_position != token_end && isBinaryOperator(**token_position)) {
      auto op = (Operator*) *token_position;
      token_position++;
      NExpression* rhs = parseSingleNumeric(token_position,
                                            token_end);
      lhs = new NBinaryOperator(*lhs, op->operatorCode, *rhs);
    }
    return lhs;
 }

  NExpression* parseSingleNumeric(TokenVector::iterator& token_position,
                                  TokenVector::iterator token_end) {
    NExpression* lhs = NULL;
    if (typeid(**token_position) == typeid(Integer)) {
      auto integer = (Integer*) *token_position;
      lhs = new NInteger(integer->value);

    } else if (typeid(**token_position) == typeid(Double)) {
      Double* dbl = (Double*) *token_position;
      lhs = new NDouble(dbl->value);

    } else {
      throw ParserException("expected a numeric!");
    }

    token_position++;
    return lhs;
  }
}

#include "./parser.hpp"
#include <stdio.h>

using namespace lexer;

namespace parser {

  NBlock* parseBlock(TokenVector::iterator& token_position,
                     TokenVector& tokens) {
    NBlock* block = new NBlock();
    while(token_position != tokens.end()) {
      NStatement* statement = parseStatement(token_position, tokens);
      block->statements.push_back(statement);
    }
    return block;
  }

  NStatement* parseStatement(TokenVector::iterator& token_position,
                             TokenVector& tokens) {
    auto token = *token_position;

    if (token == &T_RETURN) {
      token_position++;
      return new NReturn(*parseExpression(token_position, tokens));

    } else if (typeid(*token) == typeid(Identifier)) {
      auto token_as_identifier = (Identifier*) token;
      token_position++;
      auto next_token = *token_position;
      if (next_token == &T_ASSIGN) {
        // parseAssignment
        return new NAssignment(*new NIdentifier(token_as_identifier->name),
                               *new NVoid());

      } else if(next_token == &T_DECLARE) {
        // parse declaration
        return new NVariableDeclaration(*new NIdentifier(token_as_identifier->name),
                                        *new NIdentifier("void"));
      } else {
        token_position--;
        return parseExpression(token_position, tokens);
      }

    }

    throw ParserException("Looking for statement, unable to find one");
  }

  bool isNumeric(const Token& token) {
    return typeid(token) == typeid(Integer) || typeid(token) == typeid(Double);
  }

  bool isBinaryOperator(const Token& token) {
    return typeid(token) == typeid(Operator);
  }

  NExpression* parseExpression(TokenVector::iterator& token_position,
                               TokenVector& tokens) {

    if (*token_position == &T_TRUE) {
      token_position++;
      return new NBoolean(true);

    } else if (*token_position == &T_FALSE) {
      token_position++;
      return new NBoolean(false);

    } else if (isNumeric(**token_position)) {
      return parseNumeric(token_position, tokens);

    } else if (typeid(**token_position) == typeid(String)) {
      auto stringToken = (String*) *token_position;
      token_position++;
      return new NString(stringToken->value);

    } else if (typeid(**token_position) == typeid(Identifier)) {
      token_position++;
      if (*token_position == &T_LPAREN) {
        token_position--;
        return parseMethodCall(token_position, tokens);
      }
    }

    throw ParserException("Looking for expression, unable to find one");
  }

  NMethodCall* parseMethodCall(TokenVector::iterator& token_position,
                               TokenVector& tokens) {
    auto method_name = new NIdentifier(((Identifier*) *token_position)->name);
    token_position++;

    if (*token_position != &T_LPAREN) {
      throw ParserException("expected a '(' for a method call!");
    }
    token_position++;
    // YUSUKE TODO: parse arguments
    ExpressionList arguments;
    while(*token_position != &T_RPAREN) {
      arguments.push_back(parseExpression(token_position, tokens));
    }

    if (*token_position != &T_RPAREN) {
      throw ParserException("expected a ')' for a method call!");
    }

    return new NMethodCall(*method_name, arguments);
  }

  NExpression* parseNumeric(TokenVector::iterator& token_position,
                            TokenVector& tokens) {
    NExpression* lhs = parseSingleNumeric(token_position,
                                          tokens);
    while (token_position != tokens.end() && isBinaryOperator(**token_position)) {
      auto op = (Operator*) *token_position;
      token_position++;
      NExpression* rhs = parseSingleNumeric(token_position,
                                            tokens);
      lhs = new NBinaryOperator(*lhs, op->operatorCode, *rhs);
    }
    return lhs;
 }

  NExpression* parseSingleNumeric(TokenVector::iterator& token_position,
                                  TokenVector& tokens) {
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

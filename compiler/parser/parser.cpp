#include "./parser.hpp"
#include <stdio.h>

using namespace lexer;

namespace parser {

  void validateToken(TokenVector::iterator& token_position,
                     TokenVector& tokens,
                     const std::type_info* type,
                     std::string message) {
    if (token_position == tokens.end()) {
      throw ParserException(message);
    } else if (typeid(**token_position) != *type) {
      throw ParserException(message + " found " + (*token_position)->getDescription());
    }
  }

  NBlock* parseBlock(TokenVector::iterator& token_position,
                     TokenVector& tokens) {
    NBlock* block = new NBlock();
    while(token_position != tokens.end() && *token_position != &T_UNINDENT) {
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

    } else if (typeid(*token) == typeid(TypeToken)) {
      // it's the start of a method declaration when the type is first
      return parseFunctionDeclaration(token_position, tokens);

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

    std::string message("Looking for statement, unable to find one.");
    if (token_position != tokens.end()) {
      message += " found " ;
      message += (*token_position)->getDescription();
    }
    throw ParserException(message);
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

    validateToken(token_position, tokens, &typeid(Identifier),
                  "Expected a name for a method!");

    auto method_name = new NIdentifier(((Identifier*) *token_position)->name);
    token_position++;

    ExpressionList* arguments = parseArguments(token_position, tokens);

    return new NMethodCall(*method_name, *arguments);
  }

  NFunctionDeclaration* parseFunctionDeclaration(TokenVector::iterator& token_position,
                                                 TokenVector& tokens) {

    // get + check type
    if (typeid(**token_position) != typeid(TypeToken)) {
      throw ParserException("Expected a type for a method!");
    }

    auto type = new NIdentifier(((TypeToken*) *token_position)->name);
    token_position++;

    // get + check method
    if (typeid(**token_position) != typeid(Identifier)) {
      throw ParserException("Expected a name for a method!");
    }

    auto method_name = new NIdentifier(((Identifier*) *token_position)->name);
    token_position++;

    if (*token_position != &T_LPAREN) {
      throw ParserException("expected a '(' for a method declaration!");
    }
    token_position++;

    auto arguments = parseVariableList(token_position, tokens);

    validateToken(token_position, tokens, &typeid(T_RPAREN),
                  "expected a ')' for a method declaration!");
    token_position++;

    if (*token_position != &T_COLON) {
      throw ParserException("expected a ':' for a method declaration!");
    }
    token_position++;

    if (*token_position != &T_INDENT) {
      throw ParserException("expected an indent for a method declaration!");
    }
    token_position++;

    auto nblock = parseBlock(token_position, tokens);

    if (*token_position != &T_UNINDENT) {
      throw ParserException("expected an unindent for a method declaration!");
    }
    token_position++;

    return new NFunctionDeclaration(*type,
                                    *method_name,
                                    *arguments,
                                    *nblock);
  }

  VariableList* parseVariableList(TokenVector::iterator& token_position,
                                  TokenVector& tokens) {
    auto variableList = new VariableList();
    while(typeid(**token_position) == typeid(Identifier)) {
      auto identifier = new NIdentifier(((Identifier*) *token_position)->name);
      token_position++;

      if (typeid(**token_position) != typeid(TypeToken)) {
        throw ParserException("expected a type in a variable list");
      }

      auto type = new NIdentifier(((Identifier*) *token_position)->name);
      token_position++;

      variableList->push_back(new NVariableDeclaration(*identifier, *type));
    }
    return variableList;
  }

  ExpressionList* parseArguments(TokenVector::iterator& token_position,
                                 TokenVector& tokens) {
    if (*token_position != &T_LPAREN) {
      throw ParserException("expected a '(' for a method call!");
    }
    token_position++;

    ExpressionList* arguments = new ExpressionList();
    while(*token_position != &T_RPAREN) {
      arguments->push_back(parseExpression(token_position, tokens));
    }

    if (*token_position != &T_RPAREN) {
      throw ParserException("expected a ')' for a method call!");
    }
    token_position++;

    return arguments;
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

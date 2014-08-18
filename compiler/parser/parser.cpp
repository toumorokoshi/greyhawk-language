#include "./parser.hpp"
#include <stdio.h>

using namespace lexer;

namespace parser {

  void validateToken(TokenVector::iterator& token_position,
                     TokenVector& tokens,
                     L type,
                     std::string message) {

    if (token_position == tokens.end()) {
      throw ParserException(message);

    } else if ((*token_position)->type != type) {
      throw ParserException(**token_position,
                            message + " found " + (*token_position)->getDescription());

    }
  }

  bool isNumeric(const Token& token) {
    return token.type == INT || token.type == DOUBLE;
  }

  bool isBinaryOperator(const Token& token) {
    return token.type >= PLUS && token.type <= IS;
  }

  NBlock* parseBlock(TokenVector::iterator& token_position,
                     TokenVector& tokens) {
    NBlock* block = new NBlock();
    while(token_position != tokens.end() && (*token_position)->type != UNINDENT) {
      NStatement* statement = parseStatement(token_position, tokens);
      block->statements.push_back(statement);
    }
    return block;
  }

  NStatement* parseStatement(TokenVector::iterator& token_position,
                             TokenVector& tokens) {
    auto token = *token_position;

    switch (token->type) {

    case RETURN:
      token_position++;
      return new NReturn(*parseExpression(token_position, tokens));

    case TYPE:
      // it's the start of a method declaration when the type is first
      return parseFunctionDeclaration(token_position, tokens);

    case IDENTIFIER: {
      auto identifier = token;
      token_position++;
      auto next_token = *token_position;
      if (next_token->type == ASSIGN) {
        // parseAssignment
        return new NAssignment(*new NIdentifier(identifier->value),
                               *new NVoid());

      } else if(next_token->type == TYPE || next_token->type == L_BRACKET) {
        // parse declaration
        // if we see IDENT [, we can assume this is a declaration of
        // an array type.
        token_position--;
        return parseVariableDeclaration(token_position, tokens);

      } else {
        token_position--;
        return parseExpression(token_position, tokens);
      }
    }

    default:
      std::string message("Looking for statement, unable to find one.");
      if (token_position != tokens.end()) {
        message += " found " ;
        message += (*token_position)->getDescription();
      }
      throw ParserException(**token_position, message);

    }
 }

  NVariableDeclaration* parseVariableDeclaration(TokenVector::iterator& token_position,
                                                 TokenVector& tokens) {
    if ((*token_position)->type != IDENTIFIER) {
      throw ParserException(**token_position, "expected a name for a variable declaration!");
    }

    auto identifer = new NIdentifier((*token_position)->value);
    token_position++;


    auto type = parseType(token_position, tokens);

    if ((*token_position)->type != DECLARE) {
      throw ParserException(**token_position, "expected a := for a variable declaration!");
    }
    token_position++;

    auto expression = parseExpression(token_position, tokens);

    return new NVariableDeclaration(*identifer, *type, expression);
  }

  NExpression* parseExpression(TokenVector::iterator& token_position,
                               TokenVector& tokens) {

    if ((*token_position)->type == IDENTIFIER) {
      token_position++;

      if ((*token_position)->type == LPAREN) {
        token_position--;
        return parseMethodCall(token_position, tokens);
      }

      token_position--;
    }

    // in the case where the format isn't 'name(', then we're dealing with values
    NExpression* lhs = parseValue(token_position, tokens);
    while (token_position != tokens.end() && isBinaryOperator(**token_position)) {
      auto op = (*token_position)->type;
      token_position++;
      NExpression* rhs = parseValue(token_position, tokens);
      lhs = new NBinaryOperator(*lhs, op, *rhs);
    }
    return lhs;
  }

  NExpression* parseArray(TokenVector::iterator& token_position,
                          TokenVector& tokens) {
    validateToken(token_position, tokens, L_BRACKET, "expected a [ for an array!");
    token_position++;
    auto expressions = new ExpressionList();
    while ((*token_position)->type != R_BRACKET) {
      expressions->push_back(parseExpression(token_position, tokens));
    }
    token_position++;
    return new NArray(*expressions);
  }

  NExpression* parseValue(TokenVector::iterator& token_position,
                          TokenVector& tokens) {
    auto token = *token_position;
    token_position++;
    switch (token->type) {
    case TRUE:
      return new NBoolean(true);
    case FALSE:
      return new NBoolean(false);
    case STRING:
      return new NString(token->value);
    case INT:
      return new NInteger(stoi(token->value));
    case DOUBLE:
      return new NDouble(stod(token->value));
    case IDENTIFIER:
      return new NIdentifier(token->value);
    case L_BRACKET:
      token_position--;
      return parseArray(token_position, tokens);
    default:
      throw ParserException(*token, "expected a value!");
    }
  }

  NType* parseType(TokenVector::iterator& token_position,
                         TokenVector& tokens) {
    auto token = *token_position;
    switch ((*token_position)->type) {

    case TYPE: {
      auto type = new NSingleType(token->value);
      token_position++;
      return type;
    }

    case L_BRACKET: {
      token_position++;
      validateToken(token_position, tokens, TYPE, "expected a type!");
      auto arrayType = new NArrayType(*new NSingleType((*token_position)->value));
      token_position++;
      validateToken(token_position, tokens, R_BRACKET, "expected ]!");
      token_position++;
      return arrayType;
    }

    default:
      throw ParserException(**token_position, "expected a type!");
    }
  }

  NMethodCall* parseMethodCall(TokenVector::iterator& token_position,
                                        TokenVector& tokens) {

    validateToken(token_position, tokens, IDENTIFIER,
                  "expected a name for a method!");

    auto method_name = new NIdentifier((*token_position)->value);
    token_position++;

    ExpressionList* arguments = parseArguments(token_position, tokens);

    return new NMethodCall(*method_name, *arguments);
  }

  NFunctionDeclaration* parseFunctionDeclaration(TokenVector::iterator& token_position,
                                                 TokenVector& tokens) {

   // get + check type
    auto type = parseType(token_position, tokens);

    // get + check method
    if ((*token_position)->type != IDENTIFIER) {
      throw ParserException(**token_position,
                            "expected a name for a method!");
    }

    auto method_name = new NIdentifier((*token_position)->value);
    token_position++;

    if ((*token_position)->type != LPAREN) {
      throw ParserException(**token_position,
                            "expected a '(' for a method declaration!");
    }
    token_position++;

    auto arguments = parseVariableList(token_position, tokens);

    validateToken(token_position, tokens, RPAREN,
                  "expected a ')' for a method declaration!");
    token_position++;

    if ((*token_position)->type != COLON) {
      throw ParserException(**token_position, "expected a ':' for a method declaration!");
    }
    token_position++;

    if ((*token_position)->type != INDENT) {
      throw ParserException(**token_position, "expected an indent for a method declaration!");
    }
    token_position++;

    auto nblock = parseBlock(token_position, tokens);

    if ((*token_position)->type != UNINDENT) {
      throw ParserException(**token_position, "expected an unindent for a method declaration!");
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

    while((*token_position)->type == IDENTIFIER) {
      auto identifier = new NIdentifier((*token_position)->value);
      token_position++;

      auto type = parseType(token_position, tokens);

      variableList->push_back(new NVariableDeclaration(*identifier, *type));

      if ((*token_position)->type == COMMA) {
        token_position++;
      }
    }
    return variableList;
  }

  ExpressionList* parseArguments(TokenVector::iterator& token_position,
                                 TokenVector& tokens) {
    if ((*token_position)->type != LPAREN) {
      throw ParserException(**token_position, "expected a '(' for a method call!");
    }
    token_position++;

    ExpressionList* arguments = new ExpressionList();
    while((*token_position)->type != RPAREN) {
      arguments->push_back(parseExpression(token_position, tokens));

      if ((*token_position)->type != RPAREN) {
        if ((*token_position)->type != COMMA) {
          throw ParserException("expected a ',' in between arguments.");
        }
        token_position++;
      }

    }

    if ((*token_position)->type != RPAREN) {
      throw ParserException(**token_position, "expected a ')' for a method call!");
    }
    token_position++;

    return arguments;
  }
}

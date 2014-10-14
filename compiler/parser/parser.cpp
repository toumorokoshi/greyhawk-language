#include "parser.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <string>

#define debug(s);
// #define debug(s) std::cout << s << std::endl;

using namespace lexer;
using namespace VM;

namespace parser {

  void Parser::_validateToken(L type, std::string message) {
    debug("validateToken");

    if (token_position == tokens.end()) {
      debug("validateToken: at end");
      throw ParserException(message);

    } else if ((*token_position)->type != type) {
      debug("validateToken: type mismatch");
      throw ParserException(**token_position,
                            message + " found " + (*token_position)->getDescription());

    }
  }

  PBlock* Parser::parseBlock() {
    debug("parseBlock");
    auto block = new PBlock();

    while (token_position != tokens.end()
           && (*token_position)->type != UNINDENT) {
      auto statement = parseStatement();
      block->statements.push_back(statement);
    }

    debug("parseBlock: finished");
    return block;
  }

  PIfElse* Parser::parseIfElse() {
    debug("parseIfElse");

    _validateToken(IF, "expected an 'if' for an if else statement");
    token_position++;

    auto expression = parseExpression();

    _validateToken(COLON, "expected an ':' for an if else statement");
    token_position++;

    _validateToken(INDENT, "expected an indent for an if statement");
    token_position++;

    auto trueBlock = parseBlock();

    _validateToken(UNINDENT, "expected an unindent for an if statement");
    token_position++;

    _validateToken(ELSE, "expected an 'else' for an else statement");
    token_position++;

    _validateToken(COLON, "expected an ':' for an else statement");
    token_position++;

     _validateToken(INDENT, "expected an indent for an if statement");
    token_position++;

    auto falseBlock = parseBlock();

    _validateToken(UNINDENT, "expected an unindent for an if statement");
    token_position++;

    return new PIfElse(expression, trueBlock, falseBlock);

  }

  PArray* Parser::parseArray() {
    _validateToken(L_BRACKET, "expected an '[' for an array");
    token_position++;

    auto elements = new std::vector<PExpression*>;

    while ((*token_position)->type != R_BRACKET) {
      elements->push_back(parseExpression());
      if ((*token_position)->type != R_BRACKET) {
        if ((*token_position)->type != COMMA) {
          throw ParserException(**token_position,
                                "expected a ',' in between arguments.");
        }
        token_position++;
      }
    }

    _validateToken(R_BRACKET, "expected an ']' for an array");
    token_position++;

    return new PArray(*elements);
  }

  PStatement* Parser::parseStatement() {
    debug("parseStatement");
    auto token = *token_position;

    switch (token->type) {

    case TYPE:
      return parseFunctionDeclaration();

    case IDENTIFIER: {
      auto identifier = token;
      token_position++;

      if (token_position != tokens.end()) {

        switch ((*token_position)->type) {

        case DECLARE: {
          debug("pDeclare");
          token_position++; // iterate past declare
          auto* expression = parseExpression();
          return new PDeclare(identifier->value, expression);
        }

        case LPAREN:
          token_position--;
          return parseCall();

        default:
          break;

        }

      }

      token_position--;
      auto expression = parseExpression();

      switch ((*token_position)->type) {
      case ASSIGN:
        token_position++;
        return new PAssign(expression, parseExpression());

      case INCREMENT:
        token_position++;
        return new PIncrement(expression, parseExpression());

      case DECREMENT:
        token_position++;
        return new PDecrement(expression, parseExpression());

      default:
        return expression;
      }

    }

    case IF:
      return parseIfElse();

    case FOR:
      return parseForStatement();

    case L::RETURN:
      return parseReturn();

    default:
      return parseExpression();
    }
  }

  PFunctionDeclaration* Parser::parseFunctionDeclaration() {
    // skip for now. I'll add this in later.
    _validateToken(TYPE, "expected a type for a function declaration");
    auto returnType = (*token_position)->value;
    token_position++;

    _validateToken(IDENTIFIER, "expected a function name for a function declaration");
    auto functionName = (*token_position)->value;
    token_position++;

    _validateToken(LPAREN, "expected a '(' for a method call!");
    token_position++; // iterate past a left paren

    auto arguments = new PArgumentList();

    while ((*token_position)->type != RPAREN) {

      _validateToken(IDENTIFIER, "expected a variable name for a function declaration");
      auto variableName = (*token_position)->value;
      token_position++;

      _validateToken(TYPE, "expected a variable name for a function declaration");
      auto typeName = (*token_position)->value;
      token_position++;

      arguments->push_back(new PArgumentDefinition(variableName, typeName));

      if ((*token_position)->type == COMMA) {
        token_position++;
      }

    }

    _validateToken(RPAREN, "expected a ')' for a function declaration!");
    token_position++; // iterate past a right paren

    _validateToken(COLON, "expected a ':' for a function declaration!");
    token_position++; // iterate past colon

    _validateToken(INDENT, "expected an indent for an function declaration");
    token_position++;

    auto body = parseBlock();

    _validateToken(UNINDENT, "expected an unindent for an function declaration");
    token_position++;

    return new PFunctionDeclaration(returnType, functionName, *arguments, body);

  }

  PReturn* Parser::parseReturn() {
    _validateToken(L::RETURN, "expected a return for a return statment");
    token_position++;

    auto expression = parseExpression();

    return new PReturn(expression);
  }

  PStatement* Parser::parseForStatement() {
    debug("parseForStatement");

    _validateToken(FOR, "expected a 'for' for a for loop");
    token_position++;

    switch((*token_position)->type) {
    case IDENTIFIER:
      if ((*(token_position + 1))->type == IN) {
        return parseForeachLoop();
      }
    default:
      return parseForLoop();
    }
  }

  PForeachLoop* Parser::parseForeachLoop() {
    debug("parseForLoop");

    _validateToken(IDENTIFIER, "expected a identifier for a for loop");
    auto variableName = (*token_position)->value;
    token_position++;

    _validateToken(IN, "expected a in for a for loop");
    token_position++;

    auto expression = parseExpression();

    _validateToken(COLON, "expected a : for a for loop");
    token_position++;

    _validateToken(INDENT, "expected an indent for a for loop");
    token_position++;

    auto block = parseBlock();

    _validateToken(UNINDENT, "expected an unindent for a for loop");
    token_position++;

    return new PForeachLoop(variableName, expression, block);
  }

  PForLoop* Parser::parseForLoop() {
    auto initializer = parseStatement();

    _validateToken(SEMICOLON, "expected a ';' for a for loop");
    token_position++;

    auto condition = parseExpression();

    _validateToken(SEMICOLON, "expected a ';' for a for loop");
    token_position++;

    auto incrementer = parseStatement();

    _validateToken(COLON, "expected a ':' for a for loop");
    token_position++;

    _validateToken(INDENT, "expected an indent for a for loop");
    token_position++;

    auto block = parseBlock();

    _validateToken(UNINDENT, "expected an unindent for a for loop");
    token_position++;

    return new PForLoop(initializer, condition, incrementer, block);
  }

  PCall* Parser::parseClassInstantiation() {
    auto className = (*token_position)->value;
    token_position++;

    auto arguments = parseArgumentsParens();

    return new PCall(className, *arguments);

  }
};

#include "parser.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <string>

#define debug(s);
// #define debug(s) std::cout << s << std::endl;

using namespace VM;
using namespace lexer;

namespace parser {

  bool isBinaryOperator(const Token& token) {
    return token.type >= PLUS && token.type <= IS;
  }

  VMCall* generateBinaryOperation(VMExpression* lhs, const Token& op, VMExpression* rhs) {
    std::string methodName;
    switch(op.type) {
    case PLUS:
      methodName = "__add";
      break;

    case MINUS:
      methodName = "__sub";
      break;

    case MUL:
      methodName = "__mul";
      break;

    case DIV:
      methodName = "__div";
      break;

    default:
      throw ParserException(op, "unable to get binary operator!");
    }

    return new VMCall(methodName, *new std::vector<VMExpression*> {lhs, rhs});
  }

  void Parser::_validateToken(L type, std::string message) {

    if (token_position == tokens.end()) {
      throw ParserException(message);

    } else if ((*token_position)->type != type) {
      throw ParserException(**token_position,
                            message + " found " + (*token_position)->getDescription());

    }
  }

  VMBlock* Parser::parseBlock() {
    debug("parseBlock");
    auto block = new VMBlock(scope);

    while (token_position != tokens.end()
           && (*token_position)->type != UNINDENT) {
      auto statement = parseStatement();
      block->statements.push_back(statement);
    }

    debug("parseBlock: finished");
    return block;
  }

  VMStatement* Parser::parseStatement() {
    debug("parseStatement");
    auto token = *token_position;

    switch (token->type) {

    case IDENTIFIER: {
      auto identifier = token;
      token_position++;

      switch ((*token_position)->type) {

      case DECLARE: {
        token_position++; // iterate past declare
        VMExpression* expression = parseExpression();
        return new VMDeclare(identifier->value, expression);
      }

      case ASSIGN: {
        token_position++; // iterate past assign
        VMExpression* expression = parseExpression();
        return new VMAssign(identifier->value, expression);
      }

      case LPAREN:
        token_position--;
        return parseCall();

      default:
        token_position--;
        return parseExpression();

      }

    }

    default:
      return parseExpression();
    }
  }

  VMExpression* Parser::parseExpression() {
    debug("parseExpression");
    auto lhs = parseValue();
    while (token_position != tokens.end() && isBinaryOperator(**token_position)) {
      auto token = **token_position;
      token_position++;
      auto rhs = parseValue();
      lhs = generateBinaryOperation(lhs, token, rhs);
    }
    return lhs;
  }

  VMExpression* Parser::parseValue() {
    debug("parseValue");
    auto token = *token_position;
    token_position++;
    switch(token->type) {

    case TYPE:
      debug("parseValue: returning class.");
      token_position--;
      return parseClassInstantiation();

    case STRING:
      debug("parseValue: returning string.");
      return new VMConstant(new VMString(token->value));

    case INT:
      debug("parseValue: returning int.");
      return new VMConstant(new VMInt(std::stoi(token->value)));

    case IDENTIFIER:
      debug("parseValue: return identifier.");
      return new VMIdentifier(token->value);

    case TRUE:
      debug("parseValue: return true.");
      return new VMConstant(new VMBool(true));

    case FALSE:
      debug("parseValue: return false.");
      return new VMConstant(new VMBool(false));

    default:
      throw ParserException(*token, "expected value!");
    }
  };

  VMExpression* Parser::parseClassInstantiation() {
    auto className = (*token_position)->value;
    token_position++;

    _validateToken(LPAREN, "expected a '(' for a class instantiation!");
    token_position++;

    auto arguments = parseArguments();

    _validateToken(RPAREN, "expected a ')' for a class instantiation!");
    token_position++;

    return new VMCall(className, *arguments);
  }

  VMCall* Parser::parseCall() {
    debug("parseCall");
    _validateToken(IDENTIFIER, "expected a name for a method!");
    auto method_name = (*token_position)->value;
    token_position++;

    _validateToken(LPAREN, "expected a '(' for a method call!");
    token_position++;

    std::vector<VMExpression*>* arguments = parseArguments();

    _validateToken(RPAREN, "expected a ')' for a method call!");
    token_position++; // iterating passed a right paren

    debug("finished parseCall");
    return new VMCall(method_name, *arguments);
  }

  std::vector<VMExpression*>* Parser::parseArguments() {
    auto arguments = new std::vector<VMExpression*>();

    while ((*token_position)->type != RPAREN) {
      arguments->push_back(parseExpression());
      if ((*token_position)->type != RPAREN) {
        if ((*token_position)->type != COMMA) {
          throw ParserException("expected a ',' in between arguments.");
        }
        token_position++;
      }
    }

    debug("parseArguments: finished");
    return arguments;
  }
};

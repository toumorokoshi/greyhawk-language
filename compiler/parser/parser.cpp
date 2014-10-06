#include "parser.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <string>

// #define debug(s);
#define debug(s) std::cout << s << std::endl;

using namespace lexer;
using namespace VM;

namespace parser {

  bool isBinaryOperator(const Token& token) {
    return token.type >= PLUS && token.type <= IS;
  }

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

        case ASSIGN: {
          debug("pAssign");
          token_position++; // iterate past assign
          auto* expression = parseExpression();
          return new PAssign(identifier->value, expression);
        }

        case LPAREN:
          token_position--;
          return parseFunctionCall();

        default:
          break;

        }

      }

      token_position--;
      return parseExpression();

    }

    case IF:
      return parseIfElse();

    case FOR:
      return parseForLoop();

    case RETURN:
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
    _validateToken(RETURN, "expected a return for a return statment");
    token_position++;

    auto expression = parseExpression();

    return new PReturn(expression);
  }

  PForLoop* Parser::parseForLoop() {
   debug("parseForLoop");

    _validateToken(FOR, "expected a for a for loop");
    token_position++;

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

    return new PForLoop(variableName, expression, block);
  }

  PExpression* Parser::parseExpression() {
    debug("parseExpression");
    auto lhs = parseValue();
    while (token_position != tokens.end() && isBinaryOperator(**token_position)) {
      auto token = **token_position;
      token_position++;
      auto rhs = parseValue();
      lhs = new PBinaryOperation(lhs, token.type, rhs);
    }
    return lhs;
  }

  PExpression* Parser::parseValue() {
    auto baseValue = parseBaseValue();

    if (token_position != tokens.end() && (*token_position)->type == DOT) {
      return parseMethodCall(baseValue);
    }

    return baseValue;
  }

  PExpression* Parser::parseBaseValue() {
    debug("parseBaseValue");
    auto token = *token_position;
    token_position++;
    switch(token->type) {

    case TYPE:
      debug("parseBaseValue: returning class.");
      token_position--;
      return parseClassInstantiation();

    case STRING:
      debug("parseBaseValue: returning string.");
      return new PConstantString(token->value);

    case INT:
      debug("parseBaseValue: returning int.");
      return new PConstantInt(std::stoi(token->value));

    case IDENTIFIER: {
      if (token_position != tokens.end()) {
        debug("parseBaseValue: switch on identifier");

        switch((*token_position)->type) {

        case LPAREN:
          token_position--;
          return parseFunctionCall();

        default:
          break;

        }
      }

      debug("parseBaseValue: return identifier.");
      return new PIdentifier(token->value);
    }

    case TRUE:
      debug("parseBaseValue: return true.");
      return new PConstantBool(true);

    case FALSE:
      debug("parseBaseValue: return false.");
      return new PConstantBool(false);

    default:
      throw ParserException(*token, "expected value!");
    }
  };

  PFunctionCall* Parser::parseClassInstantiation() {
    auto className = (*token_position)->value;
    token_position++;

    auto arguments = parseArgumentsParens();

    return new PFunctionCall(className, *arguments);
  }

  PMethodCall* Parser::parseMethodCall(PExpression* currentValue) {
    debug("parseMethodCall");

    debug("parseMethodCall: top of the while");
    // debug(std::to_string(*token_position));
    debug(std::to_string((*token_position)->type));
    while(token_position != tokens.end() && (*token_position)->type == DOT) {

      _validateToken(DOT, "expected a . for a method call");
      token_position++;

      _validateToken(IDENTIFIER, "expected an identifier for a method call");
      debug("parsing identifier...");
      auto methodName = (*token_position)->value;
      token_position++;

      if (token_position != tokens.end() && (*token_position)->type == LPAREN) {
        auto arguments = parseArgumentsParens();

        debug("parseMethodCall: found method call, creating VMCallMethod..");
        currentValue = new PMethodCall(currentValue, methodName, *arguments);
        continue;

      } else {
        // this is something like a.foo,
        // which directly translates to a.foo()
        debug("parseMethodCall: found property, implicitly creating VMCallMethod..");
        debug(methodName);
        currentValue = new PMethodCall(currentValue, methodName, *new PExpressions());
        continue;
      }

    }

    debug("parseMethodCall: found all chained instantiations.");

    auto methodCall = dynamic_cast<PMethodCall*>(currentValue);

    if (methodCall == NULL) {
      throw ParserException("could not parse method call!");
    }

    debug("parseMethodCall: finished");
    return methodCall;

  }

  PFunctionCall* Parser::parseFunctionCall() {
    debug("parseCall");
    auto method_name = (*token_position)->value;
    token_position++;

    PExpressions* arguments = parseArgumentsParens();

    debug("finished parseCall");
    return new PFunctionCall(method_name, *arguments);
  }

  PExpressions* Parser::parseArguments() {
    auto arguments = new PExpressions();

    while ((*token_position)->type != RPAREN) {
      arguments->push_back(parseExpression());
      if ((*token_position)->type != RPAREN) {
        if ((*token_position)->type != COMMA) {
          throw ParserException(**token_position,
                                "expected a ',' in between arguments.");
        }
        token_position++;
      }
    }

    debug("parseArguments: finished");
    return arguments;
  }

  PExpressions* Parser::parseArgumentsParens() {

    _validateToken(LPAREN, "expected a '(' for a method call!");
    token_position++; // iterate past a left paren

    auto arguments = parseArguments();

    _validateToken(RPAREN, "expected a ')' for a method call!");
    token_position++; // iterat past a right paren

    return arguments;
  }

};

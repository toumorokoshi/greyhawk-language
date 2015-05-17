#include "parser.hpp"
#include <stack>

using namespace VM;
using namespace lexer;

#ifdef DEBUG
#define debug(s) std::cerr << s << std::endl;
#else
#define debug(s);
#endif


namespace parser {

  bool isBinaryOperator(const Token& token) {
    return token.type >= PLUS && token.type <= IS;
  }

  PExpression* createBinaryOp(std::stack<PExpression*>& values,
                             lexer::L op) {
    if (values.size() < 2) {
      throw ParserException("Unable to create binary operator. Not enough operands");
    }

    auto rhs = values.top();
    values.pop();
    auto lhs = values.top();
    values.pop();

    return new PBinaryOperation(lhs, op, rhs);
  }

  // parse binary operations w/ precedence
  // using the shunting yard algorithm.
  PExpression* Parser::parseBinaryOperations() {
    std::stack<PExpression*> values;
    std::stack<lexer::L> operators;

    values.push(parseValue());
    while (token_position != tokens.end() && isBinaryOperator(**token_position)) {
      debug("Parser::parseBinaryOperations: parsing token " << (*token_position)->getFullDescription());
      auto op = (*token_position)->type;

      while (true) {
        if (operators.size() == 0) {
          operators.push(op);
          break;

        } else {
          auto topOp = operators.top();
          if (lexer::opPrecedence[topOp] >= lexer::opPrecedence[op]) {
            operators.pop();
            values.push(createBinaryOp(values, topOp));

          } else {
            operators.push(op);
            break;
          }
        }
      }

      token_position++;
      values.push(parseValue());
    }

    while (operators.size() > 0) {
      auto op = operators.top();
      operators.pop();
      values.push(createBinaryOp(values, op));
    }

    // we might need to add a check if there's
    // more that 1 value in the values stack,
    // but the way it was coded, this is not necessary
    // (while loop requires a value after every binary op
    return values.top();
  }

  PExpression* Parser::parseExpression() {
    // an expression could be one of the following:
    // * a method call
    // * a value
    // * an array access
    // a binary operator
    // all of the start with a base value.
    // TODO: handle parentheses
    return parseBinaryOperations();
  }

  PExpression* Parser::parseValue() {
    auto baseValue = parseBaseValue();

    while (true) {

      if (token_position == tokens.end()) {
        return baseValue;
      }

      // now we check to see if we're
      // accessing an array or it's function call
      switch((*token_position)->type) {
      case DOT:
        baseValue = parseMethodCall(baseValue);
        break;
      case L_BRACKET:
        baseValue = parseArrayAccess(baseValue);
        break;
      default:
        return baseValue;
      }
    }
  }

  PExpression* Parser::parseMethodCall(PExpression* currentValue) {
    debug("parseMethodCall");

    debug("parseMethodCall: top of the while");

    _validateToken(DOT, "expected a . for a method call");
    token_position++;

    _validateToken(IDENTIFIER, "expected an identifier for a method call");
    debug("parsing identifier...");
    auto methodName = (*token_position)->value;
    token_position++;

    PExpressions* arguments;

    if (token_position != tokens.end() && (*token_position)->type == LPAREN) {
      debug("parseMethodCall: found method call, creating PCallMethod..");
      arguments = parseArgumentsParens();

    } else {
      // this is something like a.foo,
      // which directly translates to a.foo()
      debug("parseMethodCall: found property, implicitly creating VMCallMethod..");

      return new PPropertyAccess(currentValue, methodName);
    }

    debug("parseMethodCall: finished");

    return new PMethodCall(currentValue, methodName, *arguments);
  }

  PCall* Parser::parseCall() {
    debug("parseCall");

    _validateToken(IDENTIFIER, "expected an identifier for a function call");
    auto name = (*token_position)->value;
    token_position++;

    PExpressions* arguments = parseArgumentsParens();
    debug("finished parseCall");
    return new PCall(name, *arguments);
  }

  PArrayAccess* Parser::parseArrayAccess(PExpression* value) {
    debug("parseArrayAccess");

    _validateToken(L_BRACKET, "expected an '[' for an array access");
    token_position++;

    auto expression = parseExpression();

    _validateToken(R_BRACKET, "expected an ']' for an array access");
    token_position++;

    debug("parseArrayAccess: finished");
    return new PArrayAccess(value, expression);
  }

  PExpression* Parser::parseBaseValue() {
    debug("parseBaseValue");
    auto token = *token_position;
    token_position++;
    switch(token->type) {

    case L_BRACKET:
      debug("parseBaseValue: return array.");
      token_position--;
      return parseConstantArray();

    case TYPE: {

      auto nextTokenType = (*token_position)->type;
      token_position--;

      switch (nextTokenType) {

      case L_BRACKET:
        debug("parseBaseValue: returning empty array.");
        return parseArray();
        break;

      case LPAREN:
        debug("parseBaseValue: returning class.");
        return parseClassInstantiation();
        break;

      default:
        throw ParserException(*token,
                              "found a class name at the beginning of an expression."
                              "expected a ( or [ for a class instantiate or an array.");
      }
    }

    case L::STRING:
      debug("parseBaseValue: returning string.");
      return new PConstantString(token->value);

    case CHAR:
      debug("parseBaseValue: returning char.");
      return new PConstantChar(token->value[0]);

    case INT:
      debug("parseBaseValue: returning int.");
      return new PConstantInt(std::stoi(token->value));

    case DOUBLE:
      debug("parseBaseValue: returning double.");
      return new PConstantFloat(std::stod(token->value));

    case IDENTIFIER: {
      debug("parseBaseValue: return identifier.");

      if ((*token_position)->type == LPAREN) {
        token_position--;
        return parseCall();
      } else {
        return new PIdentifier(token->value);
      }
    }

    case TRUE:
      debug("parseBaseValue: return true.");
      return new PConstantBool(true);

    case FALSE:
      debug("parseBaseValue: return false.");
      return new PConstantBool(false);

    case LPAREN: {
      debug("parseBaseValue: return subexpression");
      auto value = parseExpression();
      _validateToken(RPAREN, "expected a ')' to close an experssion");
      token_position++;
      return value;
    }

    default:
      throw ParserException(*token, "expected value!");
    }
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

}

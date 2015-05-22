#include "exceptions.hpp"
#include "../lexer/tokens.hpp"
#include "nodes.hpp"

#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

namespace parser {

  class CType {
  public:
    std::string value;
    CType(std::string _value) : value(_value) {}
  };

  class Parser {
  public:
    lexer::TokenVector& tokens;
    lexer::TokenVector::iterator token_position;
    Parser(lexer::TokenVector& _tokens) :
      tokens(_tokens), token_position(tokens.begin()) {}

    PArray* parseArray();
    PBlock* parseBlock();
    PStatement* parseStatement();
    PClassDeclaration* parseClassDeclaration();
    PFunctionDeclaration* parseFunctionDeclaration();
    PReturn* parseReturn();
    PStatement* parseForStatement();
    PForLoop* parseForLoop();
    PForeachLoop* parseForeachLoop();
    PIfElse* parseIfElse();
    PConstantArray* parseConstantArray();
    PExpression* parseBinaryOperations();
    PExpression* parseExpression();
    PExpression* parseValue();
    PExpression* parseBaseValue();
    PCall* parseClassInstantiation();
    PCall* parseCall();
    PType* parseType();
    PTuple* parseTuple();
    PWhile* parseWhile();
    PExpression* parseMethodCall(PExpression*);
    PArrayAccess* parseArrayAccess(PExpression*);
    PExpressions* parseArguments();
    PExpressions* parseArgumentsParens();

  private:
    void _validateToken(lexer::L type, std::string message);
  };
}

#endif

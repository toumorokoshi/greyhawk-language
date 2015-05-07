#include "nodes.hpp"
#include "parser.hpp"

using namespace lexer;
using namespace VM;

namespace parser {

  YAML::Node* PArray::toYaml() {
    auto node = new YAML::Node();
    (*node)["array"]["type"] = type;
    (*node)["array"]["size"] = *size->toYaml();
    return node;
  }

  GType* PArray::getType(codegen::GScope*) {
    return getArrayType(evaluateType(type));
  }

  GIndex* PArray::generateExpression(codegen::GScope* scope,
                                     GInstructionVector& instr) {
    auto sizeType = size->getType(scope);
    if (sizeType != getInt32Type()) {
      throw ParserException("type for size of array must be an integer! found: " +
                            sizeType->name);
    }
    auto sizeObject = size->generateExpression(scope, instr);
    sizeObject = enforceLocal(scope, sizeObject, instr);

    auto arrayType = getType(scope);
    auto arrayObject = scope->allocateObject(arrayType);

    instr.push_back(GInstruction {
        ARRAY_ALLOCATE, new GOPARG[2] {
          arrayObject->registerNum, sizeObject->registerNum
    }});
    return arrayObject;
  }

  PArray* Parser::parseArray() {
    _validateToken(TYPE, "expected a class name for an array declaration");
    auto type = (*token_position)->value;
    token_position++;

    _validateToken(L_BRACKET, "expected an '[' for an array");
    token_position++;

    auto sizeExpression = parseExpression();

    _validateToken(R_BRACKET, "expected an '[' for an array");
    token_position++;

    return new PArray(type, sizeExpression);

    _validateToken(LPAREN, "expected an '(' for an array declaration");
    token_position++;

    _validateToken(RPAREN, "expected an '(' for an array declaration");
    token_position++;
  }
}

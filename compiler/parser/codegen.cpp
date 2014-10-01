#include "nodes.hpp"
#include "exceptions.hpp"

// #define debug(s);
#define debug(s) std::cout << s << std::endl;

using namespace VM;

namespace parser {

  VMClass* evaluateType(std::string typeName) {
    if (typeName == "Int") {
      return getVMIntClass();
    } else if (typeName == "Bool") {
      return getVMBoolClass();
    } else if (typeName == "String") {
      return getVMStringClass();
    }
    throw new ParserException("Cannot find class " + typeName);
  }

  VMBlock* PBlock::generate(VMScope* scope) {
    auto block = new VMBlock();
    for (auto statement : statements) {
      block->statements.push_back(statement->generateStatement(scope));
    }
    return block;
  }

  VMStatement* PDeclare::generateStatement(VMScope* scope) {
    if (scope->localTypes.find(name) != scope->localTypes.end()) {
      throw ParserException("Cannot redeclare variable " + name);
    }

    auto vmExpression = expression->generateExpression(scope);

    scope->localTypes[name] = expression->getType();
    return new VMDeclare(name, vmExpression);
  }

  VMStatement* PAssign::generateStatement(VMScope* scope) {
    if (scope->localTypes.find(name) == scope->localTypes.end()) {
      throw ParserException("Cannot assign undeclared variable " + name);
    }
    auto vmExpression = expression->generateExpression(scope);

    if (scope->localTypes[name] != expression->getType()) {
      throw ParserException("type mismatch in assignment!");
    }

    return new VMDeclare(name, vmExpression);

  }

  VMStatement* PForLoop::generateStatement(VMScope* scope) {
    VMScope forScope(scope);
    auto expression = iterableExpression->generateExpression(scope);
    forScope.localTypes[variableName] = iterableExpression->getType();
    auto vmBlock = block->generate(&forScope);
    return new VMForLoop(vmBlock, variableName, expression);
  }

  VMStatement* PFunctionDeclaration::generateStatement(VMScope* scope) {
    if (scope->localTypes.find(name) != scope->localTypes.end()) {
      throw ParserException("Cannot redeclare " + name);
    }

    VMScope functionScope(scope);

    auto vmBody = body->generate(&functionScope);
    return NULL;
  }
}

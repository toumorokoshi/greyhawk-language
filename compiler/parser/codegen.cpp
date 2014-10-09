#include "nodes.hpp"
#include "exceptions.hpp"

#define debug(s);
// #define debug(s) std::cout << s << std::endl;

using namespace VM;
using namespace lexer;

namespace parser {

  VMClass* evaluateType(std::string typeName) {
    if (typeName == "Int") {
      return getVMIntClass();
    } else if (typeName == "Bool") {
      return getVMBoolClass();
    } else if (typeName == "String") {
      return getVMStringClass();
    } else if (typeName == "None") {
      return getNoneType();
    }
    throw ParserException("Cannot find class " + typeName);
  }

  VMBlock* PBlock::generate(VMScope* scope) {
    auto block = new VMBlock();
    for (auto statement : statements) {
      block->statements.push_back(statement->generateStatement(scope));
    }
    return block;
  }

  VMStatement* PDeclare::generateStatement(VMScope* scope) {
    if (scope->getObjectType(name)) {
      throw ParserException("Cannot redeclare variable " + name);
    }

    auto vmExpression = expression->generateExpression(scope);

    scope->localTypes[name] = expression->getType(scope);
    return new VMDeclare(name, vmExpression);
  }

  VMStatement* PAssign::generateStatement(VMScope* scope) {
    auto vmExpression = expression->generateExpression(scope);

    if (auto pIdentifier = dynamic_cast<PIdentifier*>(identifier)) {
      if (!scope->getObjectType(pIdentifier->name)) {
        throw ParserException("Cannot assign undeclared variable " + pIdentifier->name);
      }

      if (!expression->getType(scope)->matches(scope->localTypes[pIdentifier->name])) {
        throw ParserException("type mismatch in assignment!");
      }

      return new VMAssign(pIdentifier->name, vmExpression);
    } else if (auto pArrayAccess = dynamic_cast<PArrayAccess*>(identifier)) {
      return new VMCallMethod(pArrayAccess->value->generateExpression(scope),
                              "__set",
                              *new std::vector<VMExpression*> {
                                pArrayAccess->index->generateExpression(scope),
                                  vmExpression
                                  });

    }
    throw ParserException("Cannot assign value!");
  }

  VMExpression* PBinaryOperation::generateExpression(VM::VMScope* scope) {
    if (!lhs->getType(scope)->matches(rhs->getType(scope))) {
      throw ParserException("Type mismatch for binary operation!");
    }

    std::string methodName = "";
    switch(op) {

    case PLUS: methodName =  "__add"; break;
    case MINUS: methodName = "__sub"; break;
    case MUL: methodName =   "__mul"; break;
    case DIV: methodName =   "__div"; break;

    default:
      throw ParserException("Cannot find operator!");
    }

    auto arguments = new std::vector<VMExpression*>;
    arguments->push_back(lhs->generateExpression(scope));
    arguments->push_back(rhs->generateExpression(scope));
    return new VMCall(methodName, *arguments);
  }

  VMStatement* PForLoop::generateStatement(VMScope* scope) {
    VMScope forScope(scope);
    auto expression = iterableExpression->generateExpression(scope);
    forScope.localTypes[variableName] = iterableExpression->getType(scope);
    auto vmBlock = block->generate(&forScope);
    return new VMForLoop(vmBlock, variableName, expression);
  }

  VMStatement* PFunctionDeclaration::generateStatement(VMScope* scope) {
    if (scope->localTypes.find(name) != scope->localTypes.end()) {
      throw ParserException("Cannot redeclare " + name);
    }

    VMScope functionScope(scope);
    for (auto argument : arguments) {
      functionScope.localTypes[argument->first] = evaluateType(argument->second);
    }

    auto vmBody = body->generate(&functionScope);

    scope->localTypes[name] = getVMFunctionClass();
    auto function = new VMFunctionDeclaration(name,
                                              evaluateType(returnType),
                                              arguments,
                                              vmBody);
    scope->locals[name] = function;
    return function;
  }

  VMStatement* PReturn::generateStatement(VMScope* scope) {
    return new VMReturn(expression->generateExpression(scope));
  }

  VMExpression* PArray::generateExpression(VMScope* scope) {
    VMClass* elementType = getNoneType();
    if (elements.size() > 0) { elementType = elements[0]->getType(scope); }

    auto vmElements = new std::vector<VMExpression*>;

    for (auto element : elements) {
      if (!elementType->matches(element->getType(scope))) {
        throw ParserException("array Element does not match type!");
      }

      vmElements->push_back(element->generateExpression(scope));
    }

    return new VMArrayExpression(elementType, *vmElements);
  }

  VMClass* PCall::getType(VM::VMScope* scope) {
    auto function = dynamic_cast<VMFunction*>(scope->getObject(name));
    return function->getType();
  }

  VMExpression* PCall::generateExpression(VMScope* scope) {
    if (!scope->getObjectType(name)) {
      throw ParserException("function does not exist in scope: " + name);
    }

    auto scopeType = scope->getObjectType(name);

    if (!getVMFunctionClass()->matches(scopeType)) {
      throw ParserException(name + " is not a function");
    }

    auto function = dynamic_cast<VMFunction*>(scope->getObject(name));


    std::vector<VMClass*> argumentTypes;
    auto argumentExpressions = new std::vector<VMExpression*>;
    for (auto argument : arguments) {
      argumentTypes.push_back(argument->getType(scope));
      argumentExpressions->push_back(argument->generateExpression(scope));
    }

    function->validateTypes(argumentTypes);

    return new VMCall(name, *argumentExpressions);
  }

  VMExpression* PArrayAccess::generateExpression(VM::VMScope* scope) {
    return new VMCallMethod(value->generateExpression(scope),
                            "__get",
                            *new std::vector<VMExpression*> { index->generateExpression(scope) });
  }

  VMExpression* PMethodCall::generateExpression(VM::VMScope* scope) {
    auto vmValue = currentValue->generateExpression(scope);
    auto vmArguments = new std::vector<VMExpression*>;
    for (auto argument : arguments) {
      vmArguments->push_back(argument->generateExpression(scope));
    }
    return new VMCallMethod(vmValue, methodName, *vmArguments);
  }
}

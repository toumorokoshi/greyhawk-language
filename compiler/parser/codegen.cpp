#include "nodes.hpp"
#include "exceptions.hpp"

#define debug(s);
// #define debug(s) std::cout << s << std::endl;

using namespace VM;
using namespace lexer;

namespace parser {

  GType* evaluateType(std::string typeName) {
    if (typeName == "Int") { return getInt32Type(); }
    else if (typeName == "Bool") { return getBoolType(); }
    else if (typeName == "String") { return getStringType(); }
    else if (typeName == "None") { return getNoneType(); }

    throw ParserException("Cannot find class " + typeName);
  }

  GInstruction* generateRoot(VM::GScope* scope, PBlock* block) {
    auto instructions = block->generate(scope);
    instructions->push_back(GInstruction { END, NULL });
    return &(*instructions)[0];
  }

  GInstructionVector* PBlock::generate(VM::GScope* scope) {
    auto instructions = new GInstructionVector;
    for (auto statement : statements) {
      statement->generateStatement(scope, *instructions);
    }
    return instructions;
  }

  GObject* PCall::generateExpression(VM::GScope* scope,
                                     GInstructionVector& instructions) {
    if (name == "print") {
      instructions.push_back(GInstruction {
          PRINT,
            new GObject*[1] {
            arguments[0]->generateExpression(scope, instructions)
              }});
      return getNoneObject();
    }
    return getNoneObject();
  }

  void PDeclare::generateStatement(VM::GScope* scope,
                                   GInstructionVector& instructions) {
    scope->locals[name] = expression->generateExpression(scope, instructions);
  }

  GObject* PBinaryOperation::generateExpression(VM::GScope* scope,
                                                GInstructionVector& instructions) {
    auto lhsObject = lhs->generateExpression(scope, instructions);
    auto rhsObject = rhs->generateExpression(scope, instructions);
    switch (op) {
    case L::LESS_THAN: {
      auto iteratorCond = new GObject {
        getBoolType(), { false }
      };
      instructions.push_back(GInstruction {
          GOPCODE::LESS_THAN, new GObject*[3] {
            lhsObject, rhsObject, iteratorCond
          }
        });
      return iteratorCond;
    }
    default:
      throw ParserException("binary op not implemented");
    }
  }

  void PIncrement::generateStatement(VM::GScope* scope,
                                     GInstructionVector& instructions) {
    auto toIncrement = identifier->generateExpression(scope, instructions);
    if (toIncrement->type != getInt32Type()) {
      throw ParserException("only supporting int32 for increment ATM");
    }

    auto incrementer = expression->generateExpression(scope, instructions);
    if (incrementer->type != getInt32Type()) {
      throw ParserException("only supporting int32 for increment ATM");
    }

    instructions.push_back(GInstruction{
        GOPCODE::ADD, new GObject*[3] {
          toIncrement, incrementer, toIncrement
        }
      });
  }

  GObject* PIdentifier::generateExpression(VM::GScope* scope,
                                           GInstructionVector& instructions) {
    auto object = scope->getObject(name);

    if (object == NULL) {
      throw ParserException("Object " + name + " is not defined in this scope!");
    }
    return object;
  }

  void PForLoop::generateStatement(VM::GScope* scope,
                                   GInstructionVector& instructions) {
    initializer->generateStatement(scope, instructions);
    auto statements = body->generate(scope);
    auto forLoopStart = instructions.size();
    instructions.reserve(instructions.size()
                         + distance(statements->begin(), statements->end()));
    instructions.insert(instructions.end(),
                        statements->begin(),
                        statements->end());
    incrementer->generateStatement(scope, instructions);
    auto conditionObject = condition->generateExpression(scope, instructions);
    instructions.push_back(GInstruction {
        GOPCODE::BRANCH, new GObject*[3] {
          conditionObject,
            new GObject { getInt32Type(), { (int) forLoopStart } },
            new GObject { getInt32Type(), { ((int) instructions.size()) + 1 }}
        }
      });
  }

  GObject* PArray::generateExpression(VM::GScope* scope,
                                      GInstructionVector& instructions) {
    auto array = new GObject*[elements.size()];
    auto type = getNoneType();
    for (int i = 0; i < elements.size(); i++) {
      array[i] = elements[i]->generateExpression(scope, instructions);
      type = array[i]->type;
    }
    auto arrayObject = new GObject { getArrayType(), { 0 } };
    arrayObject->value.asArray = new GArray {
      array, type, (int) elements.size()
    };
    return arrayObject;
  }

  GObject* PArrayAccess::generateExpression(VM::GScope* scope,
                                            GInstructionVector& instructions) {
    auto valueObject = value->generateExpression(scope, instructions);
    auto indexObject = index->generateExpression(scope, instructions);
    auto objectRegister = new GObject { valueObject->value.asArray->elementType, {0} };
    if (indexObject->type->classifier != INT32) {
      throw ParserException("index on array is not an int");
    }
    instructions.push_back(GInstruction {
        GOPCODE::ACCESS_ELEMENT, new GObject*[3] {
          valueObject, indexObject, objectRegister
        }
      });
    return objectRegister;
  }

  // generates the instructions to parse the array
  void parseArrayIterator(std::string varName, GObject* array, PBlock* body,
                          GScope* scope, GInstructionVector& instructions) {
    GScope forScope(scope);
    auto arrayValue = array->value.asArray;
    auto iteratorIndex = new GObject { getInt32Type(), {0}};
    auto iteratorObject = new GObject { arrayValue->elementType, {0}};
    auto zero = new GObject { getInt32Type(), { 0 }};
    auto one = new GObject { getInt32Type(), { 1 }};
    auto arraySize = new GObject { getInt32Type(), { arrayValue->size }};
    // initialize statement
    forScope.locals[varName] = iteratorObject;
    instructions.push_back(GInstruction { GOPCODE::SET, new GObject*[2] { zero, iteratorIndex }});

    auto forLoopStart = instructions.size();
    instructions.push_back(GInstruction { GOPCODE::ACCESS_ELEMENT, new GObject*[3] {
          array, iteratorIndex, iteratorObject
        }});
    auto statements = body->generate(&forScope);
    instructions.reserve(instructions.size()
                         + distance(statements->begin(), statements->end()));
    instructions.insert(instructions.end(),
                        statements->begin(),
                        statements->end());
    auto conditionObject = new GObject { getBoolType(), { 0 }};
    instructions.push_back(GInstruction {
        GOPCODE::ADD, new GObject*[3] { iteratorIndex, one, iteratorIndex }
      });
    instructions.push_back(GInstruction {
        GOPCODE::LESS_THAN, new GObject*[3] { iteratorIndex, arraySize, conditionObject }
      });
    instructions.push_back(GInstruction {
        GOPCODE::BRANCH, new GObject*[3] {
          conditionObject,
            new GObject { getInt32Type(), { (int) forLoopStart }},
            new GObject { getInt32Type(), { ((int) instructions.size()) + 1 }}
        }
      });
  }

  void PForeachLoop::generateStatement(VM::GScope* scope,
                                       GInstructionVector& instructions) {
    auto iterableValue = iterableExpression->generateExpression(scope, instructions);
    // if the value is an array, we iterate through the array first
    if (iterableValue->type == getArrayType()) {
      parseArrayIterator(variableName, iterableValue, block,
                         scope, instructions);
    } else {
      throw ParserException("foreach loop not yet implemented!");
    }
  }




  /* VMClass* evaluateType(std::string typeName) {
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
    } */
}

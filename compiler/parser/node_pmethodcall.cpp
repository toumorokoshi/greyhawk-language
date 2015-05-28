#include "nodes.hpp"
#include "exceptions.hpp"

using namespace VM;
using namespace codegen;

namespace parser {

  GType* PMethodCall::getType(GScope* scope) {
    auto objectType = currentValue->getType(scope);
    if (objectType == getBuiltinModuleType()) {
      return getNoneType();
    } else {
      auto function = objectType->environment->getFunction(methodName);
      if (function == NULL) {
        throw ParserException("Unable to find method " +
                              methodName + " in class " +
                              objectType->name);
      }
      return function->returnType;
    }
  }

  GIndex* PMethodCall::generateExpression(GScope* scope,
                                          GInstructionVector& instr) {
    debug("CODEGEN: PMethodCall");
    auto object = currentValue->generateExpression(scope, instr);
    object = enforceLocal(scope, object, instr);

    if (object->type->isPrimitive) {
      auto typeName = object->type->name.c_str();
      if (primitives.find(typeName) == primitives.end()) {
        throw ParserException("unable to find primitive method dict for " + object->type->name);
      }
      if (primitives[typeName].find(methodName.c_str()) == primitives[typeName].end()) {
        throw ParserException("unable to find method " + methodName +
                              " for type " + object->type->name);
      }
      auto primitiveMethod = primitives[typeName][methodName.c_str()];
      auto returnObject = scope->allocateObject(primitiveMethod.returnType);
      instr.push_back(GInstruction { GOPCODE::PRIMITIVE_METHOD_CALL, new GOPARG[4] {
            {returnObject->registerNum},
            {object->registerNum},
            {.asString = object->type->name.c_str()},
            {.asString = methodName.c_str()},
      }});
      return returnObject;
    }

    auto funcRegister = scope->allocateObject(getFunctionType());
    auto type = object->type;
    auto methodIdx = type->environment->getObject(methodName);
    instr.push_back(GInstruction {
        GOPCODE::INSTANCE_LOAD_ATTRIBUTE, new GOPARG[3] {
          funcRegister->registerNum, object->registerNum, methodIdx->registerNum
        }
    });

    GOPCODE instruction;
    GIndex* returnValue;
    auto argumentRegisters = new GOPARG[2 + arguments.size()];
    auto function = type->environment->getFunction(methodName);
    if (function->isNative) {
      instruction = GOPCODE::BUILTIN_CALL;
    } else {
      instruction = GOPCODE::FUNCTION_CALL;
    }

    if (function->argumentCount != (int) arguments.size()) {
      throw ParserException("Argument count mismatch! " +
                            std::to_string(function->argumentCount) +
                            " values passed, " +
                            std::to_string((int) arguments.size()) +
                            " expected.");
    }

    for (int i = 0; i < function->argumentCount; i++) {
      GType* expectedType = function->argumentTypes[i];
      GType* actualType = arguments[i]->getType(scope);
      if (expectedType != actualType) {
        throw ParserException("Argument types mismatch! "
                              "expected " + expectedType->name +
                              ", found " + actualType->name);
      }
    }

    returnValue = scope->allocateObject(function->returnType);
    argumentRegisters[0].registerNum = returnValue->registerNum;
    argumentRegisters[1].registerNum = funcRegister->registerNum;
    for (int i = 0; i < (int) arguments.size(); i++) {
      auto index = arguments[i]->generateExpression(scope, instr);
      index = enforceLocal(scope, index, instr);
      argumentRegisters[i + 2].registerNum = index->registerNum;
    }

    instr.push_back(GInstruction {
        instruction, argumentRegisters
    });
    return returnValue;
  }
}

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
                                          GInstructionVector& instructions) {
    auto object = currentValue->generateExpression(scope, instructions);
    object = enforceLocal(scope, object, instructions);
    debug("methodcall object: " << methodName)
    debug("  " << object->registerNum)
    debug("  " << object->indexType)
    auto funcRegister = scope->allocateObject(getFunctionType());
    auto type = object->type;
    auto methodIdx = type->environment->getObject(methodName);
    instructions.push_back(GInstruction {
        GOPCODE::INSTANCE_LOAD_ATTRIBUTE, new GOPARG[3] {
          funcRegister->registerNum, object->registerNum, methodIdx->registerNum
        }
    });

    GOPCODE instruction;
    GIndex* returnValue;
    auto argumentRegisters = new GOPARG[2 + arguments.size()];
    auto function = type->environment->getFunction(methodName);
    if (object->type == getBuiltinModuleType()) {
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
        // debug(expectedType->subTypes[0]->name);
        // debug(actualType->subTypes[0])
        // debug(actualType->subTypes[0]->name);
        throw ParserException("Argument types mismatch! "
                              "expected " + expectedType->name +
                              ", found " + actualType->name);
      }
    }

    returnValue = scope->allocateObject(function->returnType);
    argumentRegisters[0].registerNum = returnValue->registerNum;
    argumentRegisters[1].registerNum = funcRegister->registerNum;
    for (int i = 0; i < (int) arguments.size(); i++) {
      auto index = arguments[i]->generateExpression(scope, instructions);
      index = enforceLocal(scope, index, instructions);
      argumentRegisters[i + 2].registerNum = index->registerNum;
    }

    instructions.push_back(GInstruction {
        instruction, argumentRegisters
    });
    return returnValue;
  }
}

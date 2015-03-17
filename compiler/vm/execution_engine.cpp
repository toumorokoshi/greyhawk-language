#include "execution_engine.hpp"
#include <string.h>
#include <string>
#include <iostream>

#define debug(s) std::cout << s << std::endl;
// #define debug(s)

namespace VM {

  GValue executeFunction(GModules* vm, GFunction* function, GValue* arguments) {
    auto scopeInstance = function->scope->createInstance();
    auto argumentCount = function->argumentCount;
    // TODO: better copy logic
    for (int i = 0; i < argumentCount; i++) {
      scopeInstance.values[i] = arguments[i];
    }
    return executeInstructions(vm, function->instructions, scopeInstance);
  }

  /* inline GValue executeSubfunction(GModules*, GFunction* function,
                                   GValue* parentRegisters,
                                   GOPARG* values) {
    GValue registers[function->registerCount];
    auto argumentCount = function->argumentCount;
    for (int i = 0; i < argumentCount; i++) {
      // we increment by two because first two args is function pointer, return value register
      auto value = parentRegisters[values[i + 2].registerNum];
      registers[i] = value;
    }
    return executeInstructions(vm, function->instructions, registers);
    } */

  GValue executeInstructions(GModules* modules, GInstruction* instructions, GScopeInstance& scopeInstance) {
    auto registers = scopeInstance.values;
    auto scope = scopeInstance.scope;
    auto instruction = instructions;
    bool done = false;
    while (!done) {
      auto args = instruction->args;

      switch (instruction->op) {

      case ARRAY_ALLOCATE:
        registers[args[0].registerNum].asArray =
          new GArray{ new GValue[args[1].size], args[1].size };
        break;

      case ARRAY_SET_VALUE:
        registers[args[0].registerNum].asArray->elements[registers[args[1].registerNum].asInt32] =
          registers[args[2].registerNum];
        break;

      case ARRAY_LOAD_VALUE:
        registers[args[2].registerNum] =
          registers[args[0].registerNum].asArray->elements[registers[args[1].registerNum].asInt32];
        break;

      case ARRAY_LOAD_LENGTH:
        registers[args[1].registerNum].asInt32 =
          registers[args[0].registerNum].asArray->size;
        break;

      case ADD_INT:
        registers[args[2].registerNum].asInt32 =
          registers[args[0].registerNum].asInt32 + registers[args[1].registerNum].asInt32;
        break;

      case ADD_FLOAT:
        // addFloat(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case BRANCH:
        if (registers[args[0].registerNum].asBool) {
          instruction += args[1].positionDiff - 1;
        } else {
          instruction += args[2].positionDiff - 1;
        }
        break;

        /* case CALL: {
        auto function = args[0].function;
        registers[args[1].registerNum] = executeSubfunction(vm, function, registers, args);
        break;
        } */

      case DIVIDE_FLOAT:
        registers[args[2].registerNum].asFloat =
          registers[args[0].registerNum].asFloat / registers[args[1].registerNum].asFloat;
        break;

      case DIVIDE_INT:
        registers[args[2].registerNum].asInt32 =
          registers[args[0].registerNum].asInt32 / registers[args[1].registerNum].asInt32;
        break;

      case END:
        return { 0 };
        break;

      case FILEHANDLE_WRITE: {
        auto file = registers[args[0].registerNum].asFile;
        auto str = registers[args[1].registerNum].asArray;
        auto elements = str->elements;
        for (int i = 0; i < str->size; i++) {
          fprintf(file, "%c", elements[i].asChar);
        }
        break;
      }

      case GO:
        instruction += args[0].positionDiff- 1;
        break;

      case GLOBAL_LOAD:
        registers[args[0].registerNum] = \
          *(scope->globals[args[1].registerNum]);
        break;

      // INSTANCE METHODS

      case INSTANCE_CREATE: {
        auto type = registers[args[1].registerNum].asType;
        auto attributes = new GValue[type->subTypeCount];
        for (int i = 0; i < type->subTypeCount; i++) {
          attributes[i] = registers[args[i + 2].registerNum];
        }
        registers[args[0].registerNum].asInstance = new GInstance{
          .type = type,
          .attributes = attributes
        };
        break;
      }

      case INSTANCE_LOAD_ATTRIBUTE:
        registers[args[0].registerNum] = \
          registers[args[1].registerNum].asInstance->attributes[args[2].registerNum];
        break;

      case INSTANCE_STORE_ATTRIBUTE:
        registers[args[0].registerNum].asInstance->attributes[args[1].registerNum] = \
          registers[args[2].registerNum];
        break;

      case INT_TO_FLOAT:
        // intToFloat(instruction->values[0], instruction->values[1]);
        break;

      case LOAD_CONSTANT_BOOL:
        registers[args[0].registerNum].asBool = args[1].asBool;
        break;

      case LOAD_CONSTANT_FILEHANDLE:
        registers[args[0].registerNum].asFile = args[1].asFile;
        break;

      case LOAD_CONSTANT_FLOAT:
        registers[args[0].registerNum].asFloat = args[1].asFloat;
        break;

      case LOAD_CONSTANT_INT:
        registers[args[0].registerNum].asInt32 = args[1].asInt32;
        break;

      case LOAD_CONSTANT_STRING: {
        auto constantString = args[1].asString;
        auto length = (int) strlen(constantString);
        auto elements = new GValue[length];
        for (int i = 0; i < length; i++) {
          elements[i].asChar = constantString[i];
        }
        registers[args[0].registerNum].asArray =
          new GArray { .elements = elements, .size = length };
        break;
      }

      case LOAD_MODULE: {
        auto moduleName = args[1].asString;
        registers[args[0].registerNum].asModule = (*modules)[moduleName];
        break;
      }

      case LOAD_MODULE_VALUE: {
        auto name = args[1].asString;
        registers[args[2].registerNum] =
          registers[args[0].registerNum].asModule->getValue(name);
        break;
      }

      case LESS_THAN_INT:
        registers[args[2].registerNum].asBool =
          registers[args[0].registerNum].asInt32 < registers[args[1].registerNum].asInt32;
        break;

      case MULTIPLY_FLOAT:
        registers[args[2].registerNum].asFloat =
          registers[args[0].registerNum].asFloat * registers[args[1].registerNum].asFloat;
        break;

      case MULTIPLY_INT:
        registers[args[2].registerNum].asInt32 =
          registers[args[0].registerNum].asInt32 * registers[args[1].registerNum].asInt32;
        break;

      case PRINT_CHAR:
        printf("%c\n", registers[args[0].registerNum].asChar);
        break;

      case PRINT_FLOAT:
        printf("%f\n", registers[args[0].registerNum].asFloat);
        break;

      case PRINT_INT:
        printf("%d\n", registers[args[0].registerNum].asInt32);
        break;

      case PRINT_STRING: {
        auto str = registers[args[0].registerNum].asArray;
        auto elements = str->elements;
        for (int i = 0; i < str->size; i++) {
          printf("%c", elements[i].asChar);
        }
        printf("\n");
        break;
      }

      case RETURN:
        return registers[instruction->args[0].registerNum];

      case RETURN_NONE:
        return { 0 };

      case SET:
        registers[args[1].registerNum] = registers[args[0].registerNum];
        break;

      case SUBTRACT_INT:
        registers[args[2].registerNum].asInt32 =
          registers[args[0].registerNum].asInt32 - registers[args[1].registerNum].asInt32;
        break;

      case SUBTRACT_FLOAT:
        break;
      }


      instruction++;
    }
  }
}

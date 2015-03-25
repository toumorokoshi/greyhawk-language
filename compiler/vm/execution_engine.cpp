#include "execution_engine.hpp"
#include <string.h>
#include <string>
#include <iostream>

#define debug(s) std::cout << s << std::endl;
// #define debug(s)

namespace VM {

  GValue executeFunction(GModules* vm,
                         GFunction* function,
                         GScopeInstance& parent,
                         GValue* arguments) {
    auto scopeInstance = function->scope.createInstance(parent);
    auto argumentCount = function->argumentCount;
    // TODO: better copy logic
    for (int i = 0; i < argumentCount; i++) {
      scopeInstance.locals[i] = arguments[i];
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
    auto locals = scopeInstance.locals;
    auto globals = scopeInstance.globals;
    auto scope = scopeInstance.scope;
    auto instruction = instructions;
    bool done = false;
    while (!done) {
      auto args = instruction->args;

      switch (instruction->op) {

      case ARRAY_ALLOCATE:
        locals[args[0].registerNum].asArray =
          new GArray{ new GValue[args[1].size], args[1].size };
        break;

      case ARRAY_SET_VALUE:
        locals[args[0].registerNum].asArray->elements[locals[args[1].registerNum].asInt32] =
          locals[args[2].registerNum];
        break;

      case ARRAY_LOAD_VALUE:
        locals[args[2].registerNum] =
          locals[args[0].registerNum].asArray->elements[locals[args[1].registerNum].asInt32];
        break;

      case ARRAY_LOAD_LENGTH:
        locals[args[1].registerNum].asInt32 =
          locals[args[0].registerNum].asArray->size;
        break;

      case ADD_INT:
        locals[args[2].registerNum].asInt32 =
          locals[args[0].registerNum].asInt32 + locals[args[1].registerNum].asInt32;
        break;

      case ADD_FLOAT:
        // addFloat(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case BRANCH:
        if (locals[args[0].registerNum].asBool) {
          instruction += args[1].positionDiff - 1;
        } else {
          instruction += args[2].positionDiff - 1;
        }
        break;

        /* case CALL: {
        auto function = args[0].function;
        locals[args[1].registerNum] = executeSubfunction(vm, function, locals, args);
        break;
        } */

      case DIVIDE_FLOAT:
        locals[args[2].registerNum].asFloat =
          locals[args[0].registerNum].asFloat /
          locals[args[1].registerNum].asFloat;
        break;

      case DIVIDE_INT:
        locals[args[2].registerNum].asInt32 =
          locals[args[0].registerNum].asInt32 /
          locals[args[1].registerNum].asInt32;
        break;

      case END:
        return { 0 };
        break;

      case FILEHANDLE_WRITE: {
        auto file = locals[args[0].registerNum].asFile;
        auto str = locals[args[1].registerNum].asArray;
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
        locals[args[0].registerNum] = *(globals[args[1].registerNum]);
        break;

      // INSTANCE METHODS

      case INSTANCE_CREATE: {
        auto type = locals[args[1].registerNum].asType;
        auto attributes = new GValue[type->subTypeCount];
        for (int i = 0; i < type->subTypeCount; i++) {
          attributes[i] = locals[args[i + 2].registerNum];
        }
        locals[args[0].registerNum].asInstance = new GInstance{
          .type = type,
          .attributes = attributes
        };
        break;
      }

      case INSTANCE_LOAD_ATTRIBUTE:
        locals[args[0].registerNum] = \
          locals[args[1].registerNum].asInstance->attributes[args[2].registerNum];
        break;

      case INSTANCE_STORE_ATTRIBUTE:
        locals[args[0].registerNum].asInstance->attributes[args[1].registerNum] = \
          locals[args[2].registerNum];
        break;

      case INT_TO_FLOAT:
        // intToFloat(instruction->values[0], instruction->values[1]);
        break;

      case LOAD_CONSTANT_BOOL:
        locals[args[0].registerNum].asBool = args[1].asBool;
        break;

      case LOAD_CONSTANT_FILEHANDLE:
        locals[args[0].registerNum].asFile = args[1].asFile;
        break;

      case LOAD_CONSTANT_FLOAT:
        locals[args[0].registerNum].asFloat = args[1].asFloat;
        break;

      case LOAD_CONSTANT_INT:
        locals[args[0].registerNum].asInt32 = args[1].asInt32;
        break;

      case LOAD_CONSTANT_STRING: {
        auto constantString = args[1].asString;
        auto length = (int) strlen(constantString);
        auto elements = new GValue[length];
        for (int i = 0; i < length; i++) {
          elements[i].asChar = constantString[i];
        }
        locals[args[0].registerNum].asArray =
          new GArray { .elements = elements, .size = length };
        break;
      }

      case LOAD_MODULE: {
        auto moduleName = args[1].asString;
        locals[args[0].registerNum].asModule = (*modules)[moduleName];
        break;
      }

      case LOAD_MODULE_VALUE: {
        auto name = args[1].asString;
        locals[args[2].registerNum] =
          locals[args[0].registerNum].asModule->getValue(name);
        break;
      }

      case LESS_THAN_INT:
        locals[args[2].registerNum].asBool =
          locals[args[0].registerNum].asInt32 <
          locals[args[1].registerNum].asInt32;
        break;

      case MULTIPLY_FLOAT:
        locals[args[2].registerNum].asFloat =
          locals[args[0].registerNum].asFloat *
          locals[args[1].registerNum].asFloat;
        break;

      case MULTIPLY_INT:
        locals[args[2].registerNum].asInt32 =
          locals[args[0].registerNum].asInt32 *
          locals[args[1].registerNum].asInt32;
        break;

      case PRINT_CHAR:
        printf("%c\n", locals[args[0].registerNum].asChar);
        break;

      case PRINT_FLOAT:
        printf("%f\n", locals[args[0].registerNum].asFloat);
        break;

      case PRINT_INT:
        printf("%d\n", locals[args[0].registerNum].asInt32);
        break;

      case PRINT_STRING: {
        auto str = locals[args[0].registerNum].asArray;
        auto elements = str->elements;
        for (int i = 0; i < str->size; i++) {
          printf("%c", elements[i].asChar);
        }
        printf("\n");
        break;
      }

      case RETURN:
        return locals[instruction->args[0].registerNum];

      case RETURN_NONE:
        return { 0 };

      case SET:
        locals[args[1].registerNum] = locals[args[0].registerNum];
        break;

      case SUBTRACT_INT:
        locals[args[2].registerNum].asInt32 =
          locals[args[0].registerNum].asInt32 -
          locals[args[1].registerNum].asInt32;
        break;

      case SUBTRACT_FLOAT:
        break;
      }


      instruction++;
    }
  }
}

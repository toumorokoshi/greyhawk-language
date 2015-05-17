#include "execution_engine.hpp"
#include <string.h>
#include <string>
#include <iostream>

#ifdef DEBUG
  #define debug(s) std::cerr << s << std::endl;
#else
  #define debug(s);
#endif

namespace VM {

  GValue executeInstructions(GModules* modules, GInstruction* instructions, GEnvironmentInstance& environmentInstance) {
    auto locals = environmentInstance.locals;
    auto globals = environmentInstance.globals;
    auto environment = environmentInstance.environment;
    auto instruction = instructions;
    // for debugging purposes
    debug("Environment:");
    debug("  globals:");
    for (auto& kv: environment->globalsByName) {
      debug("    name: " << kv.first << " register: " << kv.second);
    }

    // the actual logic
    bool done = false;
    while (!done) {
      auto args = instruction->args;

      switch (instruction->op) {

      case ARRAY_ALLOCATE: {
        auto arraySize = locals[args[1].registerNum].asInt32;
        locals[args[0].registerNum].asArray =
          new GArray{ new GValue[arraySize], arraySize};
      }
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

      case BUILTIN_CALL: {
        debug("BUILTIN_CALL")
        auto builtin = locals[args[1].registerNum].asBuiltin;
        int argCount = 3;
        auto arguments = new GValue[3];

        for (int i = 0; i < argCount; i++) {
          // we start at argument 2 on, because 0 and 1 are the return
          // value register and the function register, respectively.
          arguments[i] = locals[args[2 + i].registerNum];
        }

        debug("BUILTIN_CALL: executing...")
        (*builtin)(arguments);
        debug("BUILTIN_CALL: finished...")
        break;
      }

      case BOOL_PRINT: {
        debug("BOOL_PRINT");
        printf("%s\n", locals[args[0].registerNum].asBool ? "true" : "false");
        break;
      }

      case CHAR_EQ:
        locals[args[2].registerNum].asBool =
          locals[args[0].registerNum].asChar ==
          locals[args[1].registerNum].asChar;
        break;

      case TYPE_LOAD:
        locals[args[0].registerNum].asType = environment->classes[args[1].registerNum];
        locals[args[0].registerNum].asType->parentEnv = &environmentInstance;
        break;

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

      case FLOAT_EQ:
        locals[args[2].registerNum].asBool =
          locals[args[0].registerNum].asFloat ==
          locals[args[1].registerNum].asFloat;

      case FUNCTION_CREATE: {
        auto function = environment->functions[args[1].registerNum];
        locals[args[0].registerNum].asFunction = \
          function->createInstance(environmentInstance);
        break;
      }

      case FUNCTION_CALL: {
        debug("FUNCTION_CALL: start " << args[1].registerNum);
        auto funcInst = locals[args[1].registerNum].asFunction;
        debug("FUNCTION_CALL: generating register num");
        auto func = funcInst->function;
        debug("FUNCTION_CALL: generating env instance");
        auto funcEnvInstance = func->environment->createInstance(funcInst->parentEnv);

        for (int i = 0; i < func->argumentCount; i++) {
          // we start at argument 2 on, because 0 and 1 are the return
          // value register and the function register, respectively.
          funcEnvInstance->locals[i] = locals[args[2 + i].registerNum];
        }

        debug("FUNCTION_CALL: execute")
        locals[args[0].registerNum] = executeInstructions(modules,
                                                         func->instructions,
                                                         *funcEnvInstance);
        delete funcEnvInstance->locals;
        delete funcEnvInstance->globals;
        delete funcEnvInstance;
        break;
      }

      case GO:
        instruction += args[0].positionDiff - 1;
        break;

      // GLOBAL METHODS

      case GLOBAL_LOAD: {
        debug("GLOBAL_LOAD")
        locals[args[0].registerNum] = *(globals[args[1].registerNum]);
        break;
      }

      case GLOBAL_SET:
        *(globals[args[0].registerNum]) = locals[args[1].registerNum];
        break;

      // INSTANCE METHODS

      case INSTANCE_CREATE: {
        auto type = locals[args[1].registerNum].asType;
        auto instance = type->instantiate();
        for (int i = 0; i < type->attributeCount; i++) {
          instance->locals[i] = locals[args[i + 2].registerNum];
        }
        locals[args[0].registerNum].asInstance = instance;
        break;
      }

      case INSTANCE_LOAD_ATTRIBUTE:
        debug("INSTANCE_LOAD_ATTRIBUTE")
        debug(locals)
        debug(locals[args[1].registerNum].asInstance)
        locals[args[0].registerNum] =                                 \
          locals[args[1].registerNum].asInstance->locals[args[2].registerNum];
        debug("finished loading attribute")
        break;

      case INSTANCE_SET_ATTRIBUTE:
        locals[args[0].registerNum].asInstance->locals[args[1].registerNum] = \
          locals[args[2].registerNum];
        break;

      case INT_TO_FLOAT:
        // intToFloat(instruction->values[0], instruction->values[1]);
        break;

      case INT_OR:
        locals[args[2].registerNum].asInt32 =
          locals[args[0].registerNum].asInt32 |
          locals[args[1].registerNum].asInt32;
        break;

      case LOAD_CONSTANT_BOOL:
        locals[args[0].registerNum].asBool = args[1].asBool;
        break;

      case LOAD_CONSTANT_CHAR:
        locals[args[0].registerNum].asChar = args[1].asChar;
        break;

      case LOAD_CONSTANT_FLOAT:
        locals[args[0].registerNum].asFloat = args[1].asFloat;
        break;

      case LOAD_CONSTANT_INT:
        debug("LOAD_CONSTANT_INT");
        locals[args[0].registerNum].asInt32 = args[1].asInt32;
        break;

      case LOAD_CONSTANT_STRING: {
        debug("LOAD_CONSTANT_STRING");
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
    return GValue { .asBool = false };
  }
}

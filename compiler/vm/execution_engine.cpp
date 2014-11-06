#include "execution_engine.hpp"
#include <string>
#include <iostream>

#define debug(s) std::cout << s << std::endl;
// #define debug(s)

namespace VM {

  GValue executeFunction(GFunction* function, GValue* arguments) {
    GValue registers[function->registerCount];
    auto argumentCount = function->argumentCount;
    // TODO: better copy logic
    for (int i = 0; i < argumentCount; i++) {
      registers[i] = arguments[i];
    }
    return executeInstructions(function->instructions, registers);
  }

  inline GValue executeSubfunction(GFunction* function, GValue* parentRegisters,
                                   GOPARG* values) {
    GValue registers[function->registerCount];
    auto argumentCount = function->argumentCount;
    for (int i = 0; i < argumentCount; i++) {
      // we increment by two because first two args is function pointer, return value register
      auto value = parentRegisters[values[i + 2].registerNum];
      registers[i] = value;
    }
    return executeInstructions(function->instructions, registers);
  }

  GValue executeInstructions(GInstruction* instructions, GValue* registers) {
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

      case CALL: {
        auto function = args[0].function;
        registers[args[1].registerNum] = executeSubfunction(function, registers, args);
        break;
      }

      case END:
        return { 0 };
        break;

      case GO:
        instruction += args[0].positionDiff- 1;
        break;

      case INT_TO_FLOAT:
        // intToFloat(instruction->values[0], instruction->values[1]);
        break;

      case LOAD_CONSTANT_BOOL:
        registers[args[0].registerNum].asBool = args[1].asBool;
        break;

      case LOAD_CONSTANT_INT:
        registers[args[0].registerNum].asInt32 = args[1].asInt32;
        break;

      case LOAD_CONSTANT_STRING:
        registers[args[0].registerNum].asString = args[1].asString;
        break;


      case LESS_THAN_INT:
        registers[args[2].registerNum].asBool =
          registers[args[0].registerNum].asInt32 < registers[args[1].registerNum].asInt32;
        break;

      case PRINT_INT:
        printf("%d\n", registers[args[0].registerNum].asInt32);
        break;

      case PRINT_STRING:
        printf("%s\n", registers[args[0].registerNum].asString);
        break;

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

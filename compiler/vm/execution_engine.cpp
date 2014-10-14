#include "execution_engine.hpp"
#include <iostream>

// #define debug(s) std::cout << s << std::endl;
#define debug(s)

namespace VM {

  inline void addInt(GObject* lhs, GObject* rhs, GObject* result) {
    result->value.asInt32 = lhs->value.asInt32 + rhs->value.asInt32;
  }

  inline void lessThan(GObject* lhs, GObject* rhs, GObject* result) {
    result->value.asBool = lhs->value.asInt32 < rhs->value.asInt32;
  }

  void print(GObject* object) {
    switch (object->type->classifier) {
    case CLASS:
      printf("class (not yet implemented)\n");
      break;
    case INT32:
      printf("%d\n", object->value.asInt32);
      break;
    case NONE:
      printf("None\n");
      break;
    case STRING:
      printf("%s\n", object->value.asString);
      break;
    }
  }

  void executeFunction(GFunction* function) {
    GInstruction* instruction = function->instructions;
    executeInstructions(function->instructions);
  }

  void executeInstructions(GInstruction* instructions) {
    auto instruction = instructions;
    bool done = false;
    while (!done) {
      switch (instruction->op) {
      case ADD:
        debug("add");
        addInt(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case BRANCH:
        debug("branch");
        if (instruction->values[0]->value.asBool) {
          instruction = &instructions[instruction->values[1]->value.asInt32 - 1];
        } else {
          instruction = &instructions[instruction->values[2]->value.asInt32 - 1];
        }
        break;

      case END:
        debug("end");
        done = true;
        break;

      case LESS_THAN:
        debug("less_than");
        lessThan(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case PRINT:
        debug("print");
        print(instruction->values[0]);
        break;
      }

      instruction++;
    }
  }
}

#include "execution_engine.hpp"
#include <string>

// #define debug(s) std::cout << s << std::endl;
#define debug(s)

namespace VM {

  inline void accessElement(GObject* array, GObject* index, GObject* result) {
    result->value = array->value.asArray->elements[index->value.asInt32]->value;
  }

  inline void addInt(GObject* lhs, GObject* rhs, GObject* result) {
    result->value.asInt32 = lhs->value.asInt32 + rhs->value.asInt32;
  }

  inline void lessThan(GObject* lhs, GObject* rhs, GObject* result) {
    result->value.asBool = lhs->value.asInt32 < rhs->value.asInt32;
  }

  inline void set(GObject* source, GObject* result) {
    result->value = source->value;
  }

  inline std::string toString(GObject* object) {
    switch (object->type->classifier) {
    case ARRAY: {
      std::string arrayString = "[";
      auto array = object->value.asArray->elements;
      auto size = object->value.asArray->size;
      for (int i = 0; i < size; i++) {
        arrayString += toString(array[i]);
        if (i < size - 1) {
          arrayString += ", ";
        }
      }
      return arrayString += "]";
    }
    case BOOL:
      return object->value.asBool ? "true" : "false";
    case CLASS:
      return "class (not yet implemented)\n";
    case INT32:
      return std::to_string(object->value.asInt32).c_str();
    case NONE:
      return "None";
    case STRING:
      return object->value.asString;
    }
    return "unable to eval type";
  }

  void print(GObject* object) {
    printf((toString(object) + "\n").c_str());
  }

  void executeFunction(GFunction* function) {
    executeInstructions(function->instructions);
  }

  void executeInstructions(GInstruction* instructions) {
    auto instruction = instructions;
    bool done = false;
    while (!done) {
      switch (instruction->op) {
      case ACCESS_ELEMENT:
        debug("access element");
        accessElement(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

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

      case SET:
        debug("set");
        set(instruction->values[0], instruction->values[1]);
        break;

      }

      instruction++;
    }
  }
}

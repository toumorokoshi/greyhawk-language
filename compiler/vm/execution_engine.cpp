#include "execution_engine.hpp"
#include <string>
#include <iostream>

// #define debug(s) std::cout << s << std::endl;
#define debug(s)

namespace VM {

  inline void accessElement(GObject* array, GObject* index, GObject* result) {
    result->value = array->value.asArray->elements[index->value.asInt32]->value;
  }

  inline void addInt(GObject* lhs, GObject* rhs, GObject* result) {
    result->value.asInt32 = lhs->value.asInt32 + rhs->value.asInt32;
  }

  inline void addFloat(GObject* lhs, GObject* rhs, GObject* result) {
    debug("lhs: " << lhs->value.asFloat);
    debug("rhs: " << rhs->value.asFloat);
    result->value.asFloat = lhs->value.asFloat + rhs->value.asFloat;
    debug("result: " << result->value.asFloat);
  }

  inline void intToFloat(GObject* integer, GObject* result) {
    result->value.asFloat = (float) integer->value.asInt32;
    debug(result->value.asFloat);
  }

  inline void length(GObject* value, GObject* result) {
    switch (value->type->classifier) {
    case ARRAY:
      result->value.asInt32 = value->value.asArray->size;
      break;
    }
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
    case FLOAT:
      return std::to_string(object->value.asFloat).c_str();
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
        debug("access element: " << instruction->values[0] << ", " << instruction->values[1] << ", " << instruction->values[2]);
        accessElement(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case ADD_INT:
        debug("add float: " << instruction->values[0] << ", " << instruction->values[1] << ", " << instruction->values[2]);
        addInt(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case ADD_FLOAT:
        debug("add float: " << instruction->values[0] << ", " << instruction->values[1] << ", " << instruction->values[2]);
        addFloat(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case BRANCH:
        debug("branch:" << instruction->values[0] << ", " << instruction->values[1] << ", " << instruction->values[2]);
        if (instruction->values[0]->value.asBool) {
          instruction += instruction->values[1]->value.asInt32 - 1;
        } else {
          instruction += instruction->values[2]->value.asInt32 - 1;
        }
        break;

      case END:
        debug("end");
        done = true;
        break;

      case INT_TO_FLOAT:
        intToFloat(instruction->values[0], instruction->values[1]);
        break;

      case LENGTH:
        debug("length: " << instruction->values[0] << ", " << instruction->values[1]);
        length(instruction->values[0], instruction->values[1]);
        break;

      case LESS_THAN:
        debug("less_than: " << instruction->values[0] << ", " << instruction->values[1] << ", " << instruction->values[2]);
        lessThan(instruction->values[0], instruction->values[1], instruction->values[2]);
        break;

      case PRINT:
        debug("print: " << instruction->values[0]);
        print(instruction->values[0]);
        break;

      case SET:
        debug("set: " << instruction->values[0] << ", " << instruction->values[1]);
        set(instruction->values[0], instruction->values[1]);
        break;

      }

      instruction++;
    }
  }
}

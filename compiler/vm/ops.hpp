#include "object.hpp"

#ifndef VM_OP_HPP
#define VM_OP_HPP


namespace VM {

  struct GFunction;

  // these are all the instruction supported by the vm.
  enum GOPCODE {
    ARRAY_ALLOCATE,
    ARRAY_SET_VALUE,
    ARRAY_LOAD_VALUE,
    ARRAY_LOAD_LENGTH,
    ADD_INT,
    ADD_FLOAT,
    CALL,
    BRANCH,
    EXECUTE,
    END,
    GO,
    INT_TO_FLOAT,
    LABEL,
    LOAD_CONSTANT_BOOL,
    LOAD_CONSTANT_FLOAT,
    LOAD_CONSTANT_INT,
    LOAD_CONSTANT_STRING,
    LENGTH,
    LESS_THAN_INT,
    PRINT_INT,
    PRINT_STRING,
    SET,
    SUBTRACT_FLOAT,
    SUBTRACT_INT,
    RETURN,
    RETURN_NONE,
  };

  typedef union {
    int registerNum;
    int positionDiff;
    int size;
    int asInt32;
    bool asBool;
    double asFloat;
    GFunction* function;
    const char* asString;
  } GOPARG;

  typedef struct {
    GOPCODE op;
    GOPARG* args;
  } GInstruction;

  typedef struct GFunction {
    GType* returnType;
    GInstruction* instructions; // array
    int registerCount;
    int argumentCount;
  } GFunction;
}

#endif

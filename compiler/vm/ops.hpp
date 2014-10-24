#ifndef VM_OP_HPP
#define VM_OP_HPP

namespace VM {

  struct GFunction;

  // these are all the instruction supported by the vm.
  enum GOPCODE {
    ACCESS_ELEMENT,
    ADD_INT,
    ADD_FLOAT,
    CALL,
    BRANCH,
    EXECUTE,
    END,
    GO,
    INT_TO_FLOAT,
    LABEL,
    LOAD_CONSTANT_INT,
    LOAD_CONSTANT_STRING,
    LENGTH,
    LESS_THAN_INT,
    PRINT_STRING,
    SET,
    RETURN,
    RETURN_NONE,
  };

  typedef union {
    int registerNum;
    int positionDiff;
    int asInt32;
    GFunction* function;
    char* asString;
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
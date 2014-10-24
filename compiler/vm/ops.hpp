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
    LENGTH,
    LESS_THAN,
    PRINT,
    SET,
    RETURN
  };

  typedef union {
    int registerNum;
    GFunction* function;
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

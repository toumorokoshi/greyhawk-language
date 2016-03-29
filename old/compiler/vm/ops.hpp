#include "object.hpp"

#ifndef VM_OP_HPP
#define VM_OP_HPP


namespace VM {

  // these are all the instruction supported by the vm.
  enum GOPCODE {
    ADD_INT,
    ADD_FLOAT,
    ARRAY_ALLOCATE,
    ARRAY_SET_VALUE,
    ARRAY_LOAD_VALUE,
    ARRAY_LOAD_LENGTH,
    BOOL_PRINT,
    BRANCH,
    BUILTIN_CALL,
    CHAR_EQ,
    DIVIDE_FLOAT,
    DIVIDE_INT,
    END,
    FILEHANDLE_WRITE,
    FLOAT_EQ,
    FUNCTION_CREATE,
    FUNCTION_CALL,
    GO,
    GLOBAL_LOAD,
    GLOBAL_SET,
    INSTANCE_CREATE,
    INSTANCE_LOAD_ATTRIBUTE,
    INSTANCE_SET_ATTRIBUTE,
    INT_TO_FLOAT,
    INT_EQ,
    INT_OR,
    LOAD_CONSTANT_BOOL,
    LOAD_CONSTANT_CHAR,
    LOAD_CONSTANT_FLOAT,
    LOAD_CONSTANT_INT,
    LOAD_CONSTANT_STRING,
    LOAD_MODULE,
    LESS_THAN_INT,
    MULTIPLY_FLOAT,
    MULTIPLY_INT,
    PRIMITIVE_METHOD_CALL,
    PRINT_CHAR,
    PRINT_FLOAT,
    PRINT_INT,
    PRINT_STRING,
    SET,
    SUBTRACT_FLOAT,
    SUBTRACT_INT,
    TYPE_LOAD,
    RETURN,
    RETURN_NONE,
  };

  typedef union {
    int registerNum;
    int positionDiff;
    int size;
    int asInt32;
    char asChar;
    bool asBool;
    double asFloat;
    FILE* asFile;
    const char* asString;
  } GOPARG;

  typedef struct {
    GOPCODE op;
    GOPARG* args;
  } GInstruction;
}

#endif

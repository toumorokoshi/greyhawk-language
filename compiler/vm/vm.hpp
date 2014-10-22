/*
  This is a new VM implementation that utilizes llvm for as the jit component

  This should use c as much as possible. c++ is only used due to the fact that
  it's the language the LLVM interface is written in.

  This is a register-based vm. The reasoning being fewer instructions, and
  matches llvm a bit better (a future choice for a jit)
 */

#include "type.hpp"
#include "object.hpp"
#include "scope.hpp"

#ifndef VM2_VM_HPP
#define VM2_VM_HPP

// no validation occurs in the vm: only execution
namespace VM {

  // these are all the instruction supported by the vm.
  enum GOPCODE {
    ACCESS_ELEMENT,
    ADD_INT,
    ADD_FLOAT,
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
    int asInt;
  };

  typedef struct {
    GOPCODE op;
    GObject** values;
  } GInstruction;

  typedef struct {
    GType* returnType;
    GInstruction* instructions; // array
    int registerCount;
    int argumentCount;
  } GFunction;

  void printInstructions(GInstruction*);
};

#endif

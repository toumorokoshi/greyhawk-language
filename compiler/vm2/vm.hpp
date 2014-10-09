/*
  This is a new VM implementation that utilizes llvm for as the jit component

  This should use c as much as possible. c++ is only used due to the fact that
  it's the language the LLVM interface is written in.
 */

#include "llvm.hpp"
#include "type.hpp"
#include "value.hpp"

#ifndef VM2_VM_HPP
#define VM2_VM_HPP

// no validation occurs in the vm: only execution
namespace VM {

  // these are all the instruction supported by the vm.
  enum GOPCODE {
    ADD,
    RETURN
  };

  typedef struct {
    GOPCODE op;
    GValue* values;
  } GInstruction;

  typedef struct {
    GType* returnType;
    GInstruction* instructions; // array
    int instructionCount;
  } GFunction;
};

#endif

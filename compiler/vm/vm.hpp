/*
  This is a new VM implementation that utilizes llvm for as the jit component

  This should use c as much as possible. c++ is only used due to the fact that
  it's the language the LLVM interface is written in.

  This is a register-based vm. The reasoning being fewer instructions, and
  matches llvm a bit better (a future choice for a jit)
 */

#include "object.hpp"
#include "ops.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "module.hpp"

#ifndef VM2_VM_HPP
#define VM2_VM_HPP

// no validation occurs in the vm: only execution
namespace VM {

  typedef struct GVM {
    std::map<std::string, GModule*> modules;
    GModule* currentModule;
  } GVM;

  void printInstructions(GInstruction*);
};

#endif

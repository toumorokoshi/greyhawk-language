#include "llvm.hpp"
#include "exception.hpp"
#include "value.hpp"
#include "vm.hpp"

#ifndef CODEGEN_JIT_HPP
#define CODEGEN_JIT_HPP

namespace VM {

  class JIT {
    std::string errStr;
    llvm::Module* module;
    llvm::ExecutionEngine& executionEngine;
    llvm::FunctionPassManager& fpm;
    llvm::IRBuilder<>& builder;
    llvm::FunctionPassManager* createFPM(llvm::Module* module);

  public:
    JIT():
      module(new llvm::Module("main", llvm::getGlobalContext())),
      executionEngine(*llvm::EngineBuilder(module).setErrorStr(&errStr).setUseMCJIT(true).create()),
      fpm(*createFPM(module)),
      builder(*new llvm::IRBuilder<>(llvm::getGlobalContext()))
    {
      if (&executionEngine == NULL) {
        throw CodeGenException("Exception with starting LLVM JIT: " + errStr);
      }
    }

    llvm::Function* compile(GFunction*);
    llvm::Value* toValue(GValue&);
    void writeInstruction(GInstruction&);
  };

}

#endif

#include "llvm.h"
#include "externs.hpp"
#include "exceptions.hpp"
#include "../parser/node.hpp"

#ifndef CODEGEN_JIT_HPP
#define CODEGEN_JIT_HPP

namespace codegen {

  class JIT {
    std::string errStr;
    llvm::Module& module;
    llvm::ExecutionEngine& executionEngine;
    llvm::FunctionPassManager& fpm;
    llvm::IRBuilder<>& builder;
    llvm::FunctionPassManager* createFPM(llvm::Module& module);

    llvm::Value* generate(NExpression&);
    llvm::Value* generate(NMethodCall&);
    llvm::Value* generate(NString&);
  public:
    JIT():
      module(*new llvm::Module("main", llvm::getGlobalContext())),
      executionEngine(*llvm::EngineBuilder(&module).setErrorStr(&errStr).create()),
      fpm(*createFPM(module)),
      builder(*new llvm::IRBuilder<>(llvm::getGlobalContext()))
    {
      if (&executionEngine == NULL) {
        throw CodeGenException("Exception with starting LLVM JIT: " + errStr);
      }
      addExterns(module);
    }

    void executeExpression(NExpression*);
  };

}

#endif

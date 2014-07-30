#include "llvm.h"
#include "externs.hpp"
#include "exceptions.hpp"
#include "../parser/node.hpp"
#include "codegenerator.hpp"

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
    CodeGenerator& codeGenerator;

  public:
    JIT():
      module(*new llvm::Module("main", llvm::getGlobalContext())),
      executionEngine(*llvm::EngineBuilder(&module).setErrorStr(&errStr).setUseMCJIT(true).create()),
      fpm(*createFPM(module)),
      builder(*new llvm::IRBuilder<>(llvm::getGlobalContext())),
      codeGenerator(*new CodeGenerator(module, fpm, builder))
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

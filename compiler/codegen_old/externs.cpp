#include "./externs.hpp"
#include <iostream>

using namespace llvm;

int32_t print(double d) {
  printf("%f\n", d);
  return 0;
}

namespace codegen {

  void addExterns(Module &module, ExecutionEngine& engine) {
    generatePuts(module, engine);
    generatePrint(module, engine);
  }

  Function* generatePuts(Module &module, ExecutionEngine& engine) {

    std::vector<Type*> arguments(1, PointerType::getUnqual(Type::getInt8Ty(getGlobalContext())));

    FunctionType* FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),
                                         arguments,
                                         false);

    return Function::Create(FT, Function::ExternalLinkage, "puts", &module);
  }

  Function* generatePrint(Module &module, ExecutionEngine& engine) {

    std::vector<Type*> arguments(1, Type::getDoubleTy(getGlobalContext()));

    FunctionType* FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),
                                         arguments,
                                         false);

    Function* f = Function::Create(FT, Function::ExternalLinkage, "print", &module);
    engine.addGlobalMapping(f, (void*) *print);
    return f;
  }

}

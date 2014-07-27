#include "./externs.hpp"

using namespace llvm;

namespace codegen {

  void addExterns(Module &module) {
    generatePuts(module);
  }

  Function* generatePuts(Module &module) {

    std::vector<Type*> arguments(1, PointerType::getUnqual(Type::getInt8Ty(getGlobalContext())));

    FunctionType* FT = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                                         arguments,
                                         false);

    return Function::Create(FT, Function::ExternalLinkage, "puts", &module);
  }

}

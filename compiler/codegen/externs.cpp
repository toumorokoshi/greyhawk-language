#include "./externs.hpp"

using namespace llvm;

namespace codegen {

  Function* generatePuts(LLVMContext& context, Module &module) {

    std::vector<Type*> arguments(1, PointerType::getUnqual(Type::getInt8Ty(context)));

    FunctionType* FT = FunctionType::get(Type::getVoidTy(context), arguments, false);

    return Function::Create(FT, Function::ExternalLinkage, "puts", &module);
  }

}

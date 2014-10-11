#include "jit.hpp"

using namespace llvm;

namespace VM {

  llvm::FunctionPassManager* JIT::createFPM(Module* module) {
    FunctionPassManager* fpm = new FunctionPassManager(module);
    fpm->add(createBasicAliasAnalysisPass());
    fpm->add(createInstructionCombiningPass());
    fpm->add(createReassociatePass());
    fpm->add(createGVNPass());
    // CFGSimplificationPass, for whatever reason, eliminates conditional branches.
    // look into NConditional to see why this is so...
    /* fpm->add(createCFGSimplificationPass()); */
    fpm->doInitialization();
    return fpm;
  }

  llvm::Type* getType(GType* type) {
    switch (type->classifier) {
    case CLASS:
      return FunctionType::getVoidTy(getGlobalContext());
    case INT32:
      return FunctionType::getInt32Ty(getGlobalContext());
    case NONE:
      return FunctionType::getVoidTy(getGlobalContext());
    }
  }

  llvm::Function* JIT::compile(GFunction* gfunction) {
    std::vector<Type*> arguments;

    FunctionType *ftype = FunctionType::get(getType(gfunction->returnType),
                                            arguments, false);

    Function* function = Function::Create(ftype, Function::ExternalLinkage,
                                          "main", module);

    BasicBlock* bblock = BasicBlock::Create(getGlobalContext(),
                                            "entry", function, 0);

    builder.SetInsertPoint(bblock);
    for (int i = 0; i < gfunction->instructionCount; i++) {
      writeInstruction(gfunction->instructions[i]);
    }

    module->dump();
    return function;
  }

  void JIT::writeInstruction(GInstruction& instruction) {
    switch (instruction.op) {
    case ADD:
    case RETURN:
      builder.CreateRet(toValue(instruction.values[0]));
    }
  }

  llvm::Value* JIT::toValue(GObject* object) {
    switch (object->type->classifier) {
    case INT32:
      return ConstantInt::get(Type::getInt32Ty(getGlobalContext()),
                              object->value.asInt32);

    case CLASS:
      return ConstantPointerNull::get(PointerType::getUnqual(Type::getVoidTy(getGlobalContext())));

    case NONE:
      return ConstantPointerNull::get(PointerType::getUnqual(Type::getVoidTy(getGlobalContext())));
    }
  }

}

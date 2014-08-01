#include "jit.hpp"
#include "exceptions.hpp"

using namespace llvm;

namespace codegen {

  void JIT::executeExpression(NExpression* expression) {

    std::vector<Type*> arguments;

    FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                                            arguments,
                                            false);

    Function* function = Function::Create(ftype, Function::ExternalLinkage, "main", &module);
    BasicBlock* bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

    builder.SetInsertPoint(bblock);
    Value* value = codeGenerator.generate(*expression);
    builder.CreateRet(value);
    module.dump();
    std::vector<GenericValue> noargs;
    GenericValue result = executionEngine.runFunction(function, noargs);

    // this is just an in-place hack.
    // 1 integer return implies that it's a "true" statement
    // everything being zero implies it's a "false" statement
    // since we're not dealing with integers properly yet, we can do this for now.
    if (result.IntVal.getLimitedValue(0xFFFFFFFF) == (uint64_t) 1) {
      std::cout << "true" << std::endl;
      // std::cout << "as int: " << result.IntVal.toString(10, true) << std::endl;
    } else if (result.DoubleVal != 0) {
      std::cout << result.DoubleVal << std::endl;
    } else {
      std::cout << "false" << std::endl;
    }

    // remove the temporary function from the body when we're done.
    function->eraseFromParent();
  }

  void JIT::runBlock(NBlock& block) {
    codeGenerator.generateRoot(block);
    Function* f = executionEngine.FindFunctionNamed("main");
    std::vector<GenericValue> noargs;
    executionEngine.runFunction(f, noargs);
  }


  FunctionPassManager* JIT::createFPM(Module& module) {
    FunctionPassManager* fpm = new FunctionPassManager(&module);
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

}

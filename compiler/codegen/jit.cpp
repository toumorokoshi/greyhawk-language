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
    Value* value = generate(*expression);
    builder.CreateRet(value);
    module.dump();
    std::vector<GenericValue> noargs;
    GenericValue result = executionEngine.runFunction(function, noargs);
    // void (*cast_fptr)() = (void (*)())(intptr_t) fptr;
    // cast_fptr();
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

  Value* JIT::generate(NExpression& expression) {
    if (typeid(expression) == typeid(NMethodCall)) {
      return generate(static_cast<NMethodCall&>(expression));

    } else if (typeid(expression) == typeid(NString)) {
      return generate(static_cast<NString&>(expression));

    }
    throw CodeGenException("Unable to evaluate expression to a type!");
  }



  Value* JIT::generate(NMethodCall& nMethodCall) {
    // check if the function exists in the current context
    Function *function = module.getFunction(nMethodCall.id.name.c_str());
    if (function == NULL) {
      throw CodeGenException("Unknown Function referenced!");
    }

    // check for argument mismatch
    if (function->arg_size() != nMethodCall.arguments.size()) {
      throw CodeGenException("Incorrect number of arguments passed!");
    }

    std::vector<Value*> args;
    for (unsigned i = 0, e = nMethodCall.arguments.size(); i != e; ++i) {
      args.push_back(generate(*(nMethodCall.arguments[i])));
      if (args.back() == 0) {
        return NULL;
      }
    }
    return builder.CreateCall(function, args, "calltmp");
  }

  Value* JIT::generate(NString& nString) {
    return builder.CreateGlobalStringPtr(*new StringRef(nString.value.c_str()), "jit");
  }

}

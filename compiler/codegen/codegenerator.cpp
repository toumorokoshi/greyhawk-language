#include "codegenerator.hpp"
#include "exceptions.hpp"
#include "../lexer/tokens.hpp"
#include <iostream>

// #define debug(s) std::cout << s << std::endl;
#define debug(s);

using namespace llvm;
using namespace lexer;

namespace codegen {

  Value* ErrorV(const char *str) { printf("Error: %s\n", str); return 0; }
  static raw_os_ostream debug_os_ostream(std::cout);
  // any value can have debug info printed with:  <Value*>->print(debug_os_ostream);

  /* Returns an LLVM type based on the identifier */
  static Type *typeOf(NIdentifier& type)
  {
    debug("generating typeof...");
    if (type.name.compare("Int") == 0) {
      return Type::getInt64Ty(getGlobalContext());
    }
    else if (type.name.compare("Double") == 0) {
      return Type::getDoubleTy(getGlobalContext());
    } else if (type.name.compare("Bool") == 0) {
      return Type::getInt1Ty(getGlobalContext());
    }
    throw CodeGenException("Unable to detect type of " + type.name);
    return Type::getVoidTy(getGlobalContext());
  }

  // utils
  bool CodeGenerator::variableExistsInContext(std::string name) {
    debug("variable exists in context...");
    debug(name);
    return getContext().locals.count(name) != 0;
  }

  BlockContext& CodeGenerator::getContext() {
    return *(blockContexts[builder.GetInsertBlock()]);
  }

  void CodeGenerator::setInsertPoint(BasicBlock* insertBlock) {
    if (blockContexts.find(insertBlock) == blockContexts.end()) {
      blockContexts[insertBlock] = new BlockContext();
    }
    builder.SetInsertPoint(insertBlock);
  }

  // generator code

  Value* CodeGenerator::generateRoot(NBlock& nblock) {
    // we create the main function
    std::vector<Type*> arguments;
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                                            arguments,
                                            false);
    Function* function = Function::Create(ftype, Function::ExternalLinkage, "main", &module);
    BasicBlock* bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);
    Value *last = NULL;
    for (StatementList::iterator it = nblock.statements.begin(); it != nblock.statements.end(); it++) {
      builder.SetInsertPoint(bblock);
      last = generate(**it);
    }
    builder.CreateRetVoid();
    return last;
  }

  Value* CodeGenerator::generate(Node& n) {
    debug("ERROR! Core node class does not have definition, and resulting in core node..");
    return NULL;
  }

  Value* CodeGenerator::generate(NExpression& n) {
    debug("dynamically determining expression");

    if (typeid(n) == typeid(NIdentifier)) {
      debug("NIdentifier");
      return generate(static_cast<NIdentifier&>(n));

    } else if (typeid(n) == typeid(NInteger)) {
      debug("NInteger");
      return generate(static_cast<NInteger&>(n));

    } else if (typeid(n) == typeid(NDouble)) {
      debug("NDouble");
      return generate(static_cast<NDouble&>(n));

    } else if (typeid(n) == typeid(NVoid)) {
      debug("NVoid");
      return generate(static_cast<NVoid&>(n));

    } else if (typeid(n) == typeid(NBoolean)) {
      debug("NBoolean");
      return generate(static_cast<NBoolean&>(n));

    } else if (typeid(n) == typeid(NMethodCall)) {
      debug("NMethodCall");
      return generate(static_cast<NMethodCall&>(n));

    } else if (typeid(n) == typeid(NBinaryOperator)) {
      debug("NBinaryOperator");
      return generate(static_cast<NBinaryOperator&>(n));

    } else if (typeid(n) == typeid(NBlock)) {
      debug("NBlock");
      return generate(static_cast<NBlock&>(n));

    } else if (typeid(n) == typeid(NString)) {
      debug("NString");
      return generate(static_cast<NString&>(n));

    }

    debug("can't determine expression!");
    return ErrorV("Unable to dynamically determine expression!");
  }

  Value* CodeGenerator::generate(NInteger& n) {
    debug("generating integer...");
    // return ConstantInt::get(getGlobalContext(), APInt(64, n.value, false));
    // TODO: actually parse ints. We'll just do doubles for now.
    return ConstantFP::get(getGlobalContext(), APFloat((double) n.value));
  }

  Value* CodeGenerator::generate(NDouble& nDouble) {
    return ConstantFP::get(getGlobalContext(), APFloat(nDouble.value));
  }

  Value* CodeGenerator::generate(NVoid& n) {
    return ConstantPointerNull::get(PointerType::getUnqual(Type::getVoidTy(getGlobalContext())));
  }

  Value* CodeGenerator::generate(NBoolean& nBoolean) {
    return nBoolean.value ? builder.getTrue() : builder.getFalse();
  }

  Value* CodeGenerator::generate(NIdentifier& nIdentifier) {
    debug("Generating identifier...");
    debug(nIdentifier.name);
    if(!variableExistsInContext(nIdentifier.name)) {
      return NULL;
    }
    return builder.CreateLoad(getContext().locals[nIdentifier.name], false);
  }

  Value* CodeGenerator::generate(NMethodCall& nMethodCall) {
    // check if the function exists in the current context
    Function *function = module.getFunction(nMethodCall.id.name.c_str());
    if (function == NULL) {
      return ErrorV("Unknown Function referenced!");
    }

    // check for argument mismatch
    if (function->arg_size() != nMethodCall.arguments.size()) {
      return ErrorV("Incorrect number of arguments passed!");
    }

    std::vector<Value*> args;
    for (unsigned i = 0, e = nMethodCall.arguments.size(); i != e; ++i) {
      args.push_back(generate(*(nMethodCall.arguments[i])));
      if (args.back() == 0) {
        return 0;
      }
    }
    return builder.CreateCall(function, args, "calltmp");
  }

  Value* CodeGenerator::generate(NBinaryOperator& n) {
    Value* l = generate(n.lhs);
    Value* r = generate(n.rhs);
    switch(n.op) {
    case OPERATOR_CODES::PLUS:                return builder.CreateFAdd(l, r, "addtmp");
    case OPERATOR_CODES::MINUS:               return builder.CreateFSub(l, r, "subtmp");
    case OPERATOR_CODES::MUL:                 return builder.CreateFMul(l, r, "multmp");
    case OPERATOR_CODES::DIV:                 return builder.CreateFDiv(l, r, "divtmp");
    case OPERATOR_CODES::COMPARE_EQUAL:       return builder.CreateFCmpOEQ(l, r, "eqtmp");
    case OPERATOR_CODES::COMPARE_NOT_EQUAL:   return builder.CreateFCmpONE(l, r, "neqtmp");
    case OPERATOR_CODES::IS:                  return builder.CreateICmpEQ(l, r, "istmp");
    default:     return ErrorV("invalid binary operator!");
    }
  }

  Value* CodeGenerator::generate(NAssignment& n) {
    if (!variableExistsInContext(n.lhs.name)) {
      return ErrorV("Undeclared variable");
    }
    debug("loading store...");
    return builder.CreateStore(generate(n.rhs), getContext().locals[n.lhs.name], false);
  }

  Value* CodeGenerator::generate(NBlock& nblock) {
    debug("Generating block");
    Value *last = NULL;
    for (StatementList::iterator it = nblock.statements.begin(); it != nblock.statements.end(); it++) {
      last = generate(**it);
    }
    return last;
  }

  Value* CodeGenerator::generate(NStatement& n) {
    debug("dynamically determining statement");

    if (typeid(n) == typeid(NConditional)) {
      debug("NConditional");
      return generate(static_cast<NConditional&>(n));

    } else if (typeid(n) == typeid(NReturn)) {
      debug("NReturn");
      return generate(static_cast<NReturn&>(n));

    } else if (typeid(n) == typeid(NAssignment)) {
      debug("NAssignment");
      return generate(static_cast<NAssignment&>(n));

    } else if (typeid(n) == typeid(NVariableDeclaration)) {
      debug("NVariableDeclaration");
      return generate(static_cast<NVariableDeclaration&>(n));

    } else if (typeid(n) == typeid(NFunctionDeclaration)) {
      debug("NFunctionDeclaration");
      return generate(static_cast<NFunctionDeclaration&>(n));

    }

    return generate(static_cast<NExpression&>(n));
  }

  Value* CodeGenerator::generate(NConditional& n) {
    // first, we get the value of the result
    debug("generating NConditional...");
    Value* conditionResult = generate(n.condition);
    Function* function = builder.GetInsertBlock()->getParent();

    debug("  creating basic blocks...");
    // then, we generate the LLVM blocks for each of the branches
    BasicBlock* thenBasicBlock = BasicBlock::Create(getGlobalContext(), "then", function);
    // we don't add the function context to else/merge until later, to keep the right order
    BasicBlock* elseBasicBlock = BasicBlock::Create(getGlobalContext(), "else");
    BasicBlock* mergeBasicBlock = BasicBlock::Create(getGlobalContext(), "merge");

    debug("  creating condition...");
    // and we create an instruction to branch them all
    builder.CreateCondBr(conditionResult, thenBasicBlock, elseBasicBlock);

    // now we fill in each of the blocks

    debug("  generating then...");
    // and we create an instruction to branch them all
    /* THEN BLOCK */
    setInsertPoint(thenBasicBlock);
    generate(n.ifBlock);
    // we always add a mergeBasicBlock at the end, to end up there.
    builder.CreateBr(mergeBasicBlock);
    // we re-assign thenBasicBlock, because it could have been modified by the inner code
    thenBasicBlock = builder.GetInsertBlock();

    debug("  generating else...");
    /* ELSE BLOCK */
    function->getBasicBlockList().push_back(elseBasicBlock);
    setInsertPoint(elseBasicBlock);
    generate(n.elseBlock);
    builder.CreateBr(mergeBasicBlock);
    elseBasicBlock = builder.GetInsertBlock();

    debug("  generating merge...");
    /* MERGE BLOCK */
    function->getBasicBlockList().push_back(mergeBasicBlock);
    setInsertPoint(mergeBasicBlock);
    return mergeBasicBlock;
  }

  Value* CodeGenerator::generate(NReturn& n) {
    debug("generating return...");
    Value* returnValue = generate(n.returnExpr);
    debug(returnValue);
    return builder.CreateRet(returnValue);
  }

  Value* CodeGenerator::generate(NVariableDeclaration& n) {
    debug("Generating NVariableDeclaration");
    AllocaInst* alloc = builder.CreateAlloca(typeOf(n.type), generate(n.id), n.id.name);
    getContext().locals[n.id.name] = alloc;
    if (n.assignmentExpr != NULL) {
      debug("generating assignment");
      NAssignment assignment(n.id, *(n.assignmentExpr));
      generate(assignment);
    }
    return alloc;
  }

  Value* CodeGenerator::generate(NFunctionDeclaration& n) {
    debug("Generating NFunctionDeclaration...");
    debug(n.id.name);

    std::vector<llvm::Type*> argTypes;
    VariableList::const_iterator it;

    debug("Getting Arguments...");
    for (it = n.arguments.begin(); it != n.arguments.end(); it++) {
      argTypes.push_back(typeOf((**it).type));
    }

    debug("Generating function objects...");
    FunctionType *ftype = FunctionType::get(typeOf(n.type), llvm::makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, Function::ExternalLinkage, n.id.name.c_str(), &module);
    BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

    debug("Setting and checking main function...");
    // if the main function, we set main function to that
    if (std::strcmp(n.id.name.c_str(), "main") == 0) {
      mainFunction = function;
    }

    debug("Setting insert block...");
    setInsertPoint(bblock);
    // Set names for all arguments.
    unsigned i = 0;
    for (Function::arg_iterator AI = function->arg_begin(); i != n.arguments.size(); ++AI, ++i) {
      AI->setName(n.arguments[i]->id.name);

      // Add arguments to variable symbol table.
      Value* allocation = generate(*(n.arguments[i]));
      builder.CreateStore(AI, allocation);
      getContext().locals[n.arguments[i]->id.name] = allocation;
    }

    debug("Generating statement...");
    Value* lastStatement = generate(n.block);
    if (!ReturnInst::classof(lastStatement)) {
      builder.CreateRetVoid();
    }

    fpm.run(*function);
    return function;
  }

  Value* CodeGenerator::generate(NString& nString) {
    return builder.CreateGlobalStringPtr(*new StringRef(nString.value.c_str()), "str");
  }

}

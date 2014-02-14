#include "codegenerator.hpp"
#include "build/parser.hpp"
#include <iostream>

using namespace llvm;

Value* ErrorV(const char *str) { printf("Error: %s\n", str); return 0; }
static raw_os_ostream debug_os_ostream(std::cout);
// any value can have debug info printed with:  <Value*>->print(debug_os_ostream);

#define debug(s) std::cout << s << std::endl;

/* Returns an LLVM type based on the identifier */
static Type *typeOf(const NIdentifier& type) 
{
  debug("generating typeof...");
	if (type.name.compare("int") == 0) {
		return Type::getInt64Ty(getGlobalContext());
	}
	else if (type.name.compare("double") == 0) {
		return Type::getDoubleTy(getGlobalContext());
	} else if (type.name.compare("bool") == 0) {
    return Type::getInt1Ty(getGlobalContext());
  }
	return Type::getVoidTy(getGlobalContext());
}

// utils
bool CodeGenerator::variableExistsInContext(std::string name) {
  debug("variable exists in context...");
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

void CodeGenerator::generateCode(NBlock& root) {
  generate(root);
  module.dump();
}

// generator code

Value* CodeGenerator::generate(Node& n) {
  debug("ERROR! Core node class doesn't not have definition, and resulting in core node..");
  return NULL;
}

Value* CodeGenerator::generate(NExpression& n) {
  if (typeid(n) == typeid(NIdentifier)) {
    return generate(static_cast<NIdentifier&>(n));

  } else if (typeid(n) == typeid(NInteger)) {
    return generate(static_cast<NInteger&>(n));


  } else if (typeid(n) == typeid(NDouble)) {
    return generate(static_cast<NDouble&>(n));

  } else if (typeid(n) == typeid(NVoid)) {
    return generate(static_cast<NVoid&>(n));

  } else if (typeid(n) == typeid(NBoolean)) {
    return generate(static_cast<NBoolean&>(n));

  } else if (typeid(n) == typeid(NMethodCall)) {
    return generate(static_cast<NMethodCall&>(n));

  } else if (typeid(n) == typeid(NBinaryOperator)) {
    return generate(static_cast<NBinaryOperator&>(n));

  } else if (typeid(n) == typeid(NAssignment)) {
    return generate(static_cast<NAssignment&>(n));

  } else if (typeid(n) == typeid(NBlock)) {
    return generate(static_cast<NBlock&>(n));
  }
  return NULL;
}

Value* CodeGenerator::generate(NInteger& n) {
  return ConstantInt::get(getGlobalContext(), APInt(64, n.value, false));
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
  case TPLUS:  return builder.CreateAdd(l, r, "addtmp");
  case TMINUS: return builder.CreateSub(l, r, "subtmp");
  case TMUL:   return builder.CreateMul(l, r, "multmp");
  case TDIV:   return builder.CreateFDiv(l, r, "divtmp");
  case TCEQ:   return builder.CreateFCmpOEQ(l, r, "eqtmp");
  case TCNE:   return builder.CreateFCmpONE(l, r, "neqtmp");
  case TIS:    return builder.CreateICmpEQ(l, r, "istmp");
  default:     return ErrorV("invalid binary operator!");
  }
}

Value* CodeGenerator::generate(NAssignment& n) {
  if (!variableExistsInContext(n.lhs.name)) {
    return ErrorV("Undeclared variable");
  }
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

  } else if (typeid(n) == typeid(NExpressionStatement)) {
    debug("NExpressionDeclaration");
    return generate(static_cast<NExpressionStatement&>(n));


  } else if (typeid(n) == typeid(NVariableDeclaration)) {
    debug("NVariableDeclaration");
    return generate(static_cast<NVariableDeclaration&>(n));

  } else if (typeid(n) == typeid(NFunctionDeclaration)) {
    debug("NFunctionDeclaration");
    return generate(static_cast<NFunctionDeclaration&>(n));
  }
  return NULL;
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

Value* CodeGenerator::generate(NExpressionStatement& n) {
  return generate(n.expression);
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



FunctionPassManager* CodeGenerator::createFPM(Module& module) {
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

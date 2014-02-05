#include "codegen.h"
#include "node.hpp"
#include "build/parser.hpp"

using namespace llvm;

static IRBuilder<> Builder(getGlobalContext());
static raw_os_ostream debug_os_ostream(std::cout);
// any value can have debug info printed with:  <Value*>->print(debug_os_ostream);

/* Returns an LLVM type based on the identifier */
static Type *typeOf(const NIdentifier& type) 
{
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

Value* ErrorV(const char *str) { printf("Error: %s\n", str); return 0; }

Value *Node::codeGen(CodeGenContext& context) {
  return NULL;
}

Value* NDouble::codeGen(CodeGenContext& context) {
  return ConstantFP::get(getGlobalContext(), APFloat(value));
}

Value* NInteger::codeGen(CodeGenContext& context) {
  return ConstantInt::get(getGlobalContext(), APInt(64, value, true));
}

Value* NIdentifier::codeGen(CodeGenContext& context) {
  if(context.locals().find(name) == context.locals().end()) {
    return NULL;
  }
  return Builder.CreateLoad(context.locals()[name], false);
}

Value* NBoolean::codeGen(CodeGenContext& context) {
  return value ? Builder.getTrue() : Builder.getFalse();
}

Value* NMethodCall::codeGen(CodeGenContext& context) {

  // check if the function exists in the current context
  Function *function = context.module->getFunction(id.name.c_str());
  if (function == NULL) {
    return ErrorV("Unknown Function referenced!");
  }

  // check for argument mismatch
  if (function->arg_size() != arguments.size()) {
    return ErrorV("Incorrect number of arguments passed!");
  }

  std::vector<Value*> args;
  for (unsigned i = 0, e = arguments.size(); i != e; ++i) {
    args.push_back(arguments[i]->codeGen(context));
    if (args.back() == 0) { 
      return 0;
    }
  }
  return Builder.CreateCall(function, args, "calltmp");
}

Value* NBinaryOperator::codeGen(CodeGenContext& context) {
  Value* l = lhs.codeGen(context);
  Value* r = rhs.codeGen(context);
  switch(op) {
  case TPLUS:  return Builder.CreateAdd(l, r, "addtmp");
  case TMINUS: return Builder.CreateSub(l, r, "subtmp");
  case TMUL:   return Builder.CreateMul(l, r, "multmp");
  case TDIV:   return Builder.CreateFDiv(l, r, "divtmp");
  case TCEQ:   return Builder.CreateFCmpOEQ(l, r, "eqtmp");
  case TCNE:   return Builder.CreateFCmpONE(l, r, "neqtmp");
  default:     return ErrorV("invalid binary operator!");
  }
}

Value* NAssignment::codeGen(CodeGenContext& context) {
  if (context.locals().find(lhs.name) == context.locals().end()) {
    return ErrorV("Undeclared variable");
  }
  return Builder.CreateStore(rhs.codeGen(context), context.locals()[lhs.name], false);
}

Value* NBlock::codeGen(CodeGenContext& context) {
  StatementList::const_iterator it;
  Value *last = NULL;
  for (it = statements.begin(); it != statements.end(); it++) {
    last = (**it).codeGen(context);
  }
  return last;
}

Value* NConditional::codeGen(CodeGenContext& context) {
  // first, we get the value of the result
  Value* conditionResult = condition->codeGen(context);
  Function* function = Builder.GetInsertBlock()->getParent();
  
  // then, we generate the LLVM blocks for each of the branches
  BasicBlock* thenBasicBlock = BasicBlock::Create(getGlobalContext(), "then", function);
  // we don't add the function context to else/merge until later, to keep the right order
  BasicBlock* elseBasicBlock = BasicBlock::Create(getGlobalContext(), "else");
  BasicBlock* mergeBasicBlock = BasicBlock::Create(getGlobalContext(), "merge");

  // and we create an instruction to branch them all
  Builder.CreateCondBr(conditionResult, thenBasicBlock, elseBasicBlock);

  // now we fill in each of the blocks

  /* THEN BLOCK */
  Builder.SetInsertPoint(thenBasicBlock);
  context.pushBlock(thenBasicBlock);
  Value* thenReturnValue = ifBlock->codeGen(context);
  // we always add a mergeBasicBlock at the end, to end up there.
  Builder.CreateBr(mergeBasicBlock);
  // we re-assign thenBasicBlock, because it could have been modified by the inner code
  thenBasicBlock = Builder.GetInsertBlock();
	context.popBlock();
 
  /* ELSE BLOCK */
  function->getBasicBlockList().push_back(elseBasicBlock);
  Builder.SetInsertPoint(elseBasicBlock);
  context.pushBlock(elseBasicBlock);
  Value* elseReturnValue = elseBlock->codeGen(context);
  Builder.CreateBr(mergeBasicBlock);
  elseBasicBlock = Builder.GetInsertBlock();

  /* MERGE BLOCK */
  function->getBasicBlockList().push_back(mergeBasicBlock);
  Builder.SetInsertPoint(mergeBasicBlock);
  PHINode *phiNode = Builder.CreatePHI(Type::getDoubleTy(getGlobalContext()), 2, "iftemp");
  phiNode->addIncoming(thenReturnValue, thenBasicBlock);
  phiNode->addIncoming(elseReturnValue, elseBasicBlock);
  return phiNode;
}

Value* NReturn::codeGen(CodeGenContext& context) {
  if (returnExpr != NULL) {
    Value* returnValue = returnExpr->codeGen(context);
    return Builder.CreateRet(returnValue);
  } else {
    return Builder.CreateRetVoid();
  }
}

Value* NExpressionStatement::codeGen(CodeGenContext& context) {
  return expression->codeGen(context);
}

Value* NVariableDeclaration::codeGen(CodeGenContext& context) {
  AllocaInst* alloc = Builder.CreateAlloca(typeOf(type), id.codeGen(context), id.name);
  context.locals()[id.name] = alloc;
  if (assignmentExpr != NULL) {
    NAssignment assignment(id, *assignmentExpr);
    assignment.codeGen(context);
  }
  return alloc;
}

Value* NFunctionDeclaration::codeGen(CodeGenContext& context) {

  std::vector<llvm::Type*> argTypes;
	VariableList::const_iterator it;

	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}

	FunctionType *ftype = FunctionType::get(typeOf(type), llvm::makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, Function::ExternalLinkage, id.name.c_str(), context.module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

  // if the main function, we set main function to that
  if (std::strcmp(id.name.c_str(), "main") == 0) {
    context.mainFunction = function;
  }

  Builder.SetInsertPoint(bblock);
	context.pushBlock(bblock);
 // Set names for all arguments.
  unsigned i = 0;
  for (Function::arg_iterator AI = function->arg_begin(); i != arguments.size(); ++AI, ++i) {
    AI->setName(arguments[i]->id.name);
    
    // Add arguments to variable symbol table.
    Value* allocation = arguments[i]->codeGen(context);
    Builder.CreateStore(AI, allocation);
    context.locals()[arguments[i]->id.name] = allocation;
  }

	Value* lastStatement = block.codeGen(context);
  if (!ReturnInst::classof(lastStatement)) {
      Builder.CreateRetVoid();
  }
	// ReturnInst::Create(getGlobalContext(), bblock);

	context.popBlock();
  context.fpm->run(*function);
	return function;
}

// CodeGenContext methods 
void CodeGenContext::generateCode(NBlock& root)
{
	/* Create the top level interpreter function to call as entry */
  
	root.codeGen(*this); /* emit bytecode for the toplevel block */
	
	/* Print the bytecode in a human-readable format 
	   to see if our program compiled properly
	 */
	PassManager pm;
  module->dump();
	// pm.add(createPrintModulePass());
	//pm.run(*module);
}

/* Executes the AST by running the main function */
void CodeGenContext::runCode() {
	/* std::cout << "Running code...\n";
	ExecutionEngine *ee = ExecutionEngine::create(module, false);
  std::vector<GenericValue> noargs;
	GenericValue v = ee->runFunction(mainFunction, noargs);
	std::cout << "Code was run.\n";
	return v; */
}

void CodeGenContext::printAST(NBlock& root) {
  root.printAST(0);
}

#include "codegen.h"
#include "node.h"
#include "build/parser.hpp"

using namespace llvm;

static IRBuilder<> Builder(getGlobalContext());
static std::map<std::string, Value*> NamedValues;

/* Returns an LLVM type based on the identifier */
static Type *typeOf(const NIdentifier& type) 
{
	if (type.name.compare("int") == 0) {
		return Type::getInt64Ty(getGlobalContext());
	}
	else if (type.name.compare("double") == 0) {
		return Type::getDoubleTy(getGlobalContext());
	}
	return Type::getVoidTy(getGlobalContext());
}

Value* ErrorV(const char *str) { printf("Error: %s\n", str); return 0; }

Value *Node::codeGen(CodeGenContext& context) {
  return NULL;
}

Value* NDouble::codeGen(CodeGenContext& context) {
  std::cout << "Creating double: " << value << std::endl;
  return ConstantFP::get(getGlobalContext(), APFloat(value));
}

Value* NInteger::codeGen(CodeGenContext& context) {
  std::cout << "Creating integer: " << value << std::endl;
  return ConstantInt::get(getGlobalContext(), APInt(64, value, true));
}

Value* NIdentifier::codeGen(CodeGenContext& context) {
  std::cout << "Creating identifier reference: " << name << std::endl;
  if(context.locals().find(name) == context.locals().end()) {
      std::cerr << "undeclared variable " << name << std::endl;
      return NULL;
  }
  return new LoadInst(context.locals()[name], "", false, context.currentBlock());
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
  std::cout << "Creating binary operation " << op << std::endl;
  Value* l = lhs.codeGen(context);
  Value* r = rhs.codeGen(context);
  switch(op) {
  case TPLUS:  return Builder.CreateAdd(l, r, "addtmp");
  case TMINUS: return Builder.CreateSub(l, r, "subtmp");
  case TMUL:   return Builder.CreateMul(l, r, "multmp");
  case TDIV:   return Builder.CreateFDiv(l, r, "divtmp");
  default:     return ErrorV("invalid binary operator!");
  }
}

Value* NAssignment::codeGen(CodeGenContext& context) {
  std::cout << "Creating assignment for " << lhs.name << std::endl;
  if (context.locals().find(lhs.name) == context.locals().end()) {
    return ErrorV("Undeclared variable");
  }
  return Builder.CreateStore(rhs.codeGen(context), context.locals()[lhs.name], false);
}

Value* NBlock::codeGen(CodeGenContext& context) {
  StatementList::const_iterator it;
  Value *last = NULL;
  for (it = statements.begin(); it != statements.end(); it++) {
    std::cout << "Generating code for " << typeid(**it).name() << std::endl;
    last = (**it).codeGen(context);
  }
  std::cout << "Creating block" << std::endl;
  return last;
}

Value* NReturn::codeGen(CodeGenContext& context) {
  if (returnExpr != NULL) {
    return Builder.CreateRet(returnExpr->codeGen(context));
  } else {
    return Builder.CreateRetVoid();
  }
}

Value* NExpressionStatement::codeGen(CodeGenContext& context) {
  std::cout << "Generating code for " << typeid(expression).name() << std::endl;
  return expression.codeGen(context);
}

Value* NVariableDeclaration::codeGen(CodeGenContext& context) {
  std::cout << nodeName() << std::endl;
  AllocaInst* alloc = Builder.CreateAlloca(typeOf(type), id.codeGen(context));
  context.locals()[id.name] = alloc;
  if (assignmentExpr != NULL) {
    NAssignment assignment(id, *assignmentExpr);
    assignment.codeGen(context);
  }
  return alloc;
}

Value *NFunctionDeclaration::codeGen(CodeGenContext& context) {

  std::vector<llvm::Type*> argTypes;
	VariableList::const_iterator it;

	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type));
	}

	FunctionType *ftype = FunctionType::get(typeOf(type), llvm::makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context.module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

	context.pushBlock(bblock);

	for (it = arguments.begin(); it != arguments.end(); it++) {
    (**it).codeGen(context);
	}

	block.codeGen(context);
	ReturnInst::Create(getGlobalContext(), bblock);

	context.popBlock();
	std::cout << "Creating function: " << id.name << std::endl;
	return function;
}

// CodeGenContext methods 
void CodeGenContext::generateCode(NBlock& root)
{
	std::cout << "Generating code...\n";
	
	/* Create the top level interpreter function to call as entry */
  std::vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Builder.getVoidTy(), makeArrayRef(argTypes), false);
	mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
	
	/* Push a new variable/block context */
	pushBlock(bblock);
	root.codeGen(*this); /* emit bytecode for the toplevel block */
	ReturnInst::Create(getGlobalContext(), bblock);
	popBlock();
	
	/* Print the bytecode in a human-readable format 
	   to see if our program compiled properly
	 */
	std::cout << "Code is generated.\n";
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

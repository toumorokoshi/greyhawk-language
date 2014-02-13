#include <stack>
#include <typeinfo>
#include <cstdio>
#include <cstring>
#include "llvm.h"
#include "node.hpp"

using namespace llvm;

// context for a basic block
typedef std::map<std::string, Value*> LocalsMap;

class BlockContext {
public:
  LocalsMap locals;
  BlockContext() {}
};

class BlockStack {
public:
  void push(BasicBlock*);
  BasicBlock* pop();
private:
  std::vector<BasicBlock*> _stack;
};

typedef std::map<BasicBlock*, BlockContext*> BlockContextMap;

class CodeGenerator {
public:
  CodeGenerator() : 
    module(*(new Module("main", getGlobalContext()))), 
    fpm(*createFPM(module)),
    builder(*(new IRBuilder<>(getGlobalContext())))
  {}
  void generateCode(NBlock&);

private:
  Module& module;
  Function* mainFunction;
  FunctionPassManager& fpm;
  IRBuilder<>& builder;
  BlockContextMap blockContexts;

  FunctionPassManager* createFPM(Module&);
 
  // utils
  bool variableExistsInContext(std::string);
  BlockContext& getContext();
  void setInsertPoint(BasicBlock*);

  // value configuration
  Value* generate(Node& n);
  Value* generate(NExpression& n);
  Value* generate(NInteger& n);
  Value* generate(NDouble& n);
  Value* generate(NVoid& n);
  Value* generate(NBoolean& n);
  Value* generate(NIdentifier& n);
  Value* generate(NMethodCall& n);
  Value* generate(NBinaryOperator& n);
  Value* generate(NAssignment& n);
  Value* generate(NBlock& n);
  Value* generate(NStatement& n);
  Value* generate(NConditional& n);
  Value* generate(NReturn& n);
  Value* generate(NExpressionStatement& n);
  Value* generate(NVariableDeclaration& n);
  Value* generate(NFunctionDeclaration& n);
};

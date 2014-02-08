#include <stack>
#include <typeinfo>
#include <cstdio>
#include <cstring>
#include "llvm.h"

using namespace llvm;

class NBlock;

class CodeGenBlock {
public:
    BasicBlock *block;
    std::map<std::string, Value*> locals;
};

class CodeGenContext {
    std::stack<CodeGenBlock *> blocks;

public:
    Function *mainFunction;
    FunctionPassManager *fpm; // optimizer for function code
    Module *module;
    CodeGenContext() { 
      module = new Module("main", getGlobalContext()); 
      fpm = createFPM(module);
    }
    
    void generateCode(NBlock& root);
    void printAST(NBlock& root);
    void runCode();
    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    void pushBlock(BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }

    FunctionPassManager* createFPM(Module* module) {
      FunctionPassManager* fpm = new FunctionPassManager(module);
      /* fpm->add(createBasicAliasAnalysisPass());
      fpm->add(createInstructionCombiningPass());
      fpm->add(createReassociatePass());
      fpm->add(createGVNPass()); */
      // CFGSimplificationPass, for whatever reason, eliminates conditional branches.
      // look into NConditional to see why this is so...
      /* fpm->add(createCFGSimplificationPass()); */
      fpm->doInitialization();
      return fpm;
    }
};

#include <stack>
#include <typeinfo>
#include <cstdio>
#include <cstring>
#include "externs.hpp"
#include "llvm.h"
#include "../parser/node.hpp"

#ifndef CODEGEN_CODEGENERATOR_HPP
#define CODEGEN_CODEGENERATOR_HPP

namespace codegen {

  class Class {
  public:
    llvm::Type* type;
    std::map<std::string, int>& attributes;
    Class(llvm::Type* _type,
          std::map<std::string, int>& _attributes) :
      type(_type),
      attributes(_attributes) {}
  };

  // context for a basic block
  typedef std::map<std::string, llvm::Value*> LocalsMap;
  typedef std::map<std::string, Class*> ClassMap;

  class BlockContext {
  public:
    LocalsMap locals;
    ClassMap classes;
    BlockContext() {}
  };

  extern llvm::Type* singleTypeOf(NSingleType& type, ClassMap& classes);
  extern llvm::Type* typeOf(NType* type, ClassMap& classes);


  typedef std::map<llvm::BasicBlock*, BlockContext*> BlockContextMap;

  class CodeGenerator {
  public:
    llvm::Module& module;

    CodeGenerator(llvm::Module& module,
                  llvm::FunctionPassManager& fpm,
                  llvm::IRBuilder<>& builder):
      module(module),
      fpm(fpm),
      builder(builder) {}

    // value configuration
    llvm::Value* generateRoot(NBlock&);
    llvm::Value* generate(Node&);
    llvm::Value* generate(NExpression&);
    // llvm::Value* generate(NArray&);
    llvm::Constant* generate(NInteger&);
    llvm::Constant* generate(NDouble&);
    llvm::Value* generate(NString&);
    llvm::Value* generate(NVoid&);
    llvm::Value* generate(NBoolean&);
    llvm::Value* generate(NIdentifier&);
    llvm::Value* generate(NMethodCall&);
    llvm::Value* generate(NBinaryOperator&);
    llvm::Value* generate(NAssignment&);
    llvm::Value* generate(NBlock&);
    llvm::Value* generate(NClassInstantiation&);
    llvm::Value* generate(NStatement&);
    llvm::Value* generate(NConditional&);
    llvm::Value* generate(NReturn&);
    llvm::Value* generate(NVariableDeclaration&);
    llvm::Value* generate(NFunctionDeclaration&);
    llvm::StructType* generate(NClassDeclaration&);

  private:
    llvm::Function* mainFunction;
    llvm::FunctionPassManager& fpm;
    llvm::IRBuilder<>& builder;
    BlockContextMap blockContexts;

    // utils
    bool variableExistsInContext(std::string);
    BlockContext& getContext();
    void setInsertPoint(llvm::BasicBlock*);

 };

}

#endif

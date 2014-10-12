#include "execution_engine.hpp"
#include <iostream>

using namespace VM;

void initializeLLVM() {
  // llvm::InitializeNativeTargetAsmPrinter();
  // llvm::InitializeNativeTargetAsmParser();
}



int _main(int argc, char const *argv[]) {

  // auto jit = new JIT();
  auto intType = new GType { BASICTYPES::INT32, "Int" };
  auto stringType = new GType { BASICTYPES::STRING, "String" };
  auto boolType = new GType { BASICTYPES::BOOL, "Bool" };

  auto one = new GObject { intType , { 1 }};
  auto iterator = new GObject { intType, { 0 } };
  auto oneHundred = new GObject { intType, { 10000 } };

  auto branchPosition = new GObject { intType, { 0 } };
  auto donePosition = new GObject { intType, { 4 } };

  auto helloWorld = new GObject { stringType, { 0 }};
  helloWorld->value.asString = "hello, world";

  auto iteratorCond = new GObject { boolType, { false }};


  auto instructions = new GInstruction[5]{
    GInstruction { GOPCODE::PRINT , new GObject*[1]{ helloWorld }},
    GInstruction { GOPCODE::ADD, new GObject*[3]{ iterator, one, iterator }},
    GInstruction { GOPCODE::LESS_THAN, new GObject*[3]{ iterator, oneHundred, iteratorCond }},
    GInstruction { GOPCODE::BRANCH, new GObject*[3] { iteratorCond, branchPosition, donePosition }},
    GInstruction { GOPCODE::END , new GObject*[3] {}}
  };

  auto printFunction = new GFunction {
    .returnType = new GType { BASICTYPES::NONE, "None" },
    .instructions = instructions,
    .instructionCount = 2
  };

  executeFunction(printFunction);
}

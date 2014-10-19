#include "execution_engine.hpp"
#include <iostream>

using namespace VM;


void VM::printInstructions(GInstruction* firstInstruction) {
  auto done = false;
  int instructionCount = 0;
  while (!done) {
    if (instructionCount < 10) {
      std::cout << "0";
    }
    std::cout << instructionCount << ", ";
    auto values = firstInstruction->values;

    switch(firstInstruction->op) {
    case ACCESS_ELEMENT:
      std::cout << "ACCESS_ELEMENT: " << values[0] << ", " << values[1] << ", " << values[2];
      break;

    case ADD:
      std::cout << "ADD: " << values[0] << ", " << values[1] << ", " << values[2];
      break;

    case BRANCH:
      std::cout << "BRANCH: " << values[0] << ", " << values[1]->value.asInt32 << ", " << values[2]->value.asInt32;
      break;

    case END:
      done = true;
      std::cout << "END";
      break;

    case LENGTH:
      std::cout << "LENGTH: " << values[0] << ", " << values[1];
      break;

    case LESS_THAN:
      std::cout << "LESS_THAN: " << values[0] << ", " << values[1] << ", " << values[2];
      break;

    case PRINT:
      std::cout << "PRINT: " << values[0];
      break;

    case SET:
      std::cout << "SET: " << values[0] << ", " << values[1];
      break;

    }
    firstInstruction++;
    instructionCount++;
    std::cout << std::endl;
  }
}

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

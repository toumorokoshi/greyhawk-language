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
    auto values = firstInstruction->args;

    switch(firstInstruction->op) {
    case ACCESS_ELEMENT:
      // std::cout << "ACCESS_ELEMENT: " << values[0] << ", " << values[1] << ", " << values[2];
      break;

    case ADD_INT:
      // std::cout << "ADD_INT: " << values[0] << ", " << values[1] << ", " << values[2];
      break;

    case ADD_FLOAT:
      // std::cout << "ADD_FLOAT: " << values[0] << ", " << values[1] << ", " << values[2];
      break;

    case BRANCH:
      // std::cout << "BRANCH: " << values[0] << ", " << values[1]->value.asInt32 << ", " << values[2]->value.asInt32;
      break;

    case END:
      done = true;
      std::cout << "END";
      break;

    case GO:
      // std::cout << "GO: " << values[0]->value.asInt32;
      break;

    case INT_TO_FLOAT:
      // std::cout << "INT_TO_FLOAT: " << values[0] << ", " << values[1];
      break;

    case LENGTH:
      // std::cout << "LENGTH: " << values[0] << ", " << values[1];
      break;

    case LESS_THAN_INT:
      // std::cout << "LESS_THAN: " << values[0] << ", " << values[1] << ", " << values[2];
      break;

    case PRINT_STRING:
      // std::cout << "PRINT: " << values[0];
      break;

    case SET:
      // std::cout << "SET: " << values[0] << ", " << values[1];
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

  auto function = new GFunction {
    getNoneType(),
    new GInstruction[9] {
      GInstruction { GOPCODE::LOAD_CONSTANT_INT, new GOPARG[2] { 0, 0 } },
      GInstruction { GOPCODE::LOAD_CONSTANT_INT, new GOPARG[2] { 1, 1 } },
      GInstruction { GOPCODE::LOAD_CONSTANT_INT, new GOPARG[2] { 2, 10000 } },
      GInstruction { GOPCODE::LOAD_CONSTANT_STRING, new GOPARG[2] { 3, GOPARG { .asString = (char*) "hello world" }}},
      GInstruction { GOPCODE::PRINT_STRING, new GOPARG[1] { 3 } },
      GInstruction { GOPCODE::ADD_INT, new GOPARG[3] { 0, 1, 0 } },
      GInstruction { GOPCODE::LESS_THAN_INT, new GOPARG[3] { 0, 2, 4 } },
      GInstruction { GOPCODE::BRANCH, new GOPARG[3] { 4, -3, 1 } },
      GInstruction { GOPCODE::RETURN_NONE, NULL}
    }, 5, 0
  };

  executeFunction(function, new GValue[0]);

  return 0;
}

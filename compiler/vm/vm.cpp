#include "execution_engine.hpp"
#include "vm.hpp"
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

    case ARRAY_ALLOCATE:
      std::cout << "ARRAY_ALLOCATE: [" << values[1].size << "] -> {" << values[0].registerNum << "}";
      break;

    case ARRAY_SET_VALUE:
      std::cout << "ARRAY_SET_VALUE: {" << values[2].registerNum << "} -> {"  << values[0].registerNum << "}[{" << values[1].registerNum << "}]";
      break;

    case ARRAY_LOAD_VALUE:
      std::cout << "ARRAY_LOAD_VALUE: {" << values[2].registerNum << "} <- {"  << values[0].registerNum << "}[{" << values[1].registerNum << "}]";
      break;

    case ARRAY_LOAD_LENGTH:
      std::cout << "ARRAY_LOAD_LENGTH: {" << values[0].registerNum << "}.length -> {"  << values[1].registerNum << "}";
      break;

    case ADD_INT:
      std::cout << "ADD_INT: {" << values[0].registerNum << "} + {" << values[1].registerNum << "} -> {" << values[2].registerNum << "}";
      break;

    case ADD_FLOAT:
      std::cout << "ADD_FLOAT";
      break;

    case BRANCH:
      std::cout << "BRANCH: {" << values[0].registerNum << "} ? " << values[1].positionDiff << " : " << values[2].positionDiff;
      break;

      /* case CALL: {
      auto function = values[0].function;
      std::cout << "CALL: " << values[0].function << "(";
      for (int i = 0; i < function->argumentCount; i++) {
        std::cout << "[" << values[2 + i].registerNum << +"]";
        if (i < function->argumentCount - 1) { std::cout << ", "; }
      }
      std::cout << ") -> [" << values[1].registerNum << "]";
      break;
      } */

    case END:
      std::cout << "END";
      done = true;
      break;

    case FUNCTION_CREATE:
      std::cout << "FUNCTION_CREATE: {" << values[0].registerNum << "} <- "
                << values[1].registerNum;
      break;

    case FUNCTION_CALL:
      std::cout << "FUNCTION_CALL: {" << values[0].registerNum << "} <- " << "{" << values[1].registerNum << "}()";
      break;

    case GO:
      std::cout << "GO: " << values[0].positionDiff;
      break;

    case GLOBAL_LOAD:
      std::cout << "GLOBAL_LOAD: {" << values[0].registerNum << "}"
                << " <- {-" << values[1].registerNum << "}";
      break;

    case INT_TO_FLOAT:
      std::cout << "INT_TO_FLOAT:";
      break;

    case INSTANCE_CREATE:
      std::cout << "INSTANCE_CREATE: {" << values[0].registerNum << "} <- {"
                << values[1].registerNum << "}()";
      break;

    case INSTANCE_LOAD_ATTRIBUTE:
      std::cout << "INSTANCE_LOAD_ATTRIBUTE: {" << values[0].registerNum << "} <- {"
                << values[1].registerNum << "}[" << values[2].registerNum << "]";
      break;

    case LENGTH:
      std::cout << "LENGTH:";
      break;

    case LOAD_CONSTANT_BOOL:
      std::cout << "LOAD_CONSTANT_BOOL: [" << values[0].registerNum << "] <- " << (values[1].asBool == true ? "true" : "false");
      break;

    case LOAD_CONSTANT_FLOAT:
      std::cout << "LOAD_CONSTANT_FLOAT: [" << values[0].registerNum << "] <- " << values[1].asFloat;
      break;

    case LOAD_CONSTANT_INT:
      std::cout << "LOAD_CONSTANT_INT: {" << values[0].registerNum << "} <- " << values[1].asInt32;
      break;

    case LOAD_CONSTANT_STRING:
      std::cout << "LOAD_CONSTANT_STRING: {" << values[0].registerNum << "} <- \"" << values[1].asString << "\"";
      break;

    case LESS_THAN_INT:
      std::cout << "LESS_THAN_INT: {" << values[0].registerNum << "} < {" << values[1].registerNum << "} -> " << values[2].registerNum;
      break;

    case MULTIPLY_FLOAT:
      std::cout << "MULTIPLY_FLOAT: {" << values[0].registerNum << "} * {" << values[1].registerNum << "} -> " << values[2].registerNum;
      break;

    case PRINT_CHAR:
      std::cout << "PRINT_CHAR: {" << values[0].registerNum << "}";
      break;

    case PRINT_FLOAT:
      std::cout << "PRINT_FLOAT: {" << values[0].registerNum << "}";
      break;

    case PRINT_INT:
      std::cout << "PRINT_INT: {" << values[0].registerNum << "}";
      break;

    case PRINT_STRING:
      std::cout << "PRINT_STRING: {" << values[0].registerNum << "}";
      break;

    case RETURN:
      std::cout << "RETURN: [" << values[0].registerNum << "]";
      break;

    case RETURN_NONE:
      std::cout << "RETURN_NONE";
      break;

    case SET:
      std::cout << "SET: {" << values[0].registerNum << "} -> {" << values[1].registerNum << "}";
      break;

    case SUBTRACT_INT:
      std::cout << "SUBTRACT_INT: {" << values[0].registerNum << "} - {" << values[1].registerNum << "} -> {" << values[2].registerNum << "}";
      break;

    case TYPE_LOAD:
      std::cout << "TYPE_LOAD: {" << values[0].registerNum << "} <- types[" << values[1].registerNum << "]";
      break;

    default:
      std::cout << "unable to print opcode: " << firstInstruction->op;
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

#include <gtest/gtest.h>
#include "../../vm/execution_engine.hpp"

using namespace VM;

TEST(VM, filehandle_write) {

  auto instructions = new GInstruction[4] {
    GInstruction { GOPCODE::LOAD_CONSTANT_STRING, new GOPARG[2] {0, GOPARG { .asString = (char*) "hello world" }}},
    GInstruction { GOPCODE::LOAD_CONSTANT_FILEHANDLE, new GOPARG[2] {1, GOPARG { .asFile = stdout }}},
    GInstruction { GOPCODE::FILEHANDLE_WRITE, new GOPARG[2] {1, 0}},
    GInstruction { END, NULL }
  };
  auto registers = new GValue[2];

  executeInstructions(NULL, instructions, registers);
}

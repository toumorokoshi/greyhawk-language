#include <gtest/gtest.h>
#include "../../vm/vm.hpp"
#include "../../vm/execution_engine.hpp"

using namespace VM;


TEST(VM, array_access) {
  auto instructions = new GInstruction[10] {
    GInstruction { ARRAY_ALLOCATE, new GOPARG[2] { 0, 2 }},

    GInstruction { LOAD_CONSTANT_INT, new GOPARG[2] { 1, 10 }},
    GInstruction { LOAD_CONSTANT_INT, new GOPARG[2] { 2, 0 }},
    GInstruction { ARRAY_SET_VALUE, new GOPARG[3] { 0, 2, 1 }},

    GInstruction { LOAD_CONSTANT_INT, new GOPARG[2] { 3, 20 }},
    GInstruction { LOAD_CONSTANT_INT, new GOPARG[2] { 4, 1 }},
    GInstruction { ARRAY_SET_VALUE, new GOPARG[3] { 0, 4, 3 }},

    GInstruction { ARRAY_LOAD_VALUE, new GOPARG[3] { 0, 4, 5 }},
    GInstruction { PRINT_INT, new GOPARG[1] { 5 } },
    GInstruction { END, NULL }
  };
  auto registers = new GValue[6];
  GEnvironmentInstance scope {
    .locals = registers
  };
  executeInstructions(NULL, instructions, scope);
}

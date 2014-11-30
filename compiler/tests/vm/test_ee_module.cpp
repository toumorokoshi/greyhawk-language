#include <gtest/gtest.h>
#include "../../vm/execution_engine.hpp"

using namespace VM;

TEST(VM, module_load) {
  auto module = new GModule();
  module->globals["foo"] = GValue{10};
  auto vm = new GVM();
  vm->modules["sys"] = module;

  auto instructions = new GInstruction[3] {
      GInstruction { GOPCODE::LOAD_MODULE, new GOPARG[2] {0, GOPARG { .asString = (char*) "sys" }}},
      GInstruction { GOPCODE::LOAD_MODULE_VALUE, new GOPARG[3] {0, GOPARG { .asString = (char*) "foo" }, 1}},
      GInstruction { END, NULL }
  };
  auto registers = new GValue[2];

  executeInstructions(vm, instructions, registers);
  EXPECT_EQ(registers[1].asInt32, 10);
}

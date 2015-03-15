#include <gtest/gtest.h>
#include "../../vm/execution_engine.hpp"
/*
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

TEST(VM, default_module_load) {
  auto module = new GModule();

  auto fooType = new GType {
    BASICTYPES::INSTANCE, "FooType", new GType*[1] { getBoolType() }, 1
  };
  module->globals["Foo"].asType = fooType;

  auto vm = new GVM();
  vm->currentModule = module;

  auto instructions = new GInstruction[2] {
    GInstruction {GOPCODE::GLOBAL_LOAD, new GOPARG[2] {
        {0}, GOPARG {.asString = (char*) "Foo"}}
    },
    GInstruction { END, NULL }
  };

  auto registers = new GValue[1];

  executeInstructions(vm, instructions, registers);
  EXPECT_EQ(registers[0].asType, fooType);
}
*/

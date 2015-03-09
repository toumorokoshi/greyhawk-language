#include <gtest/gtest.h>
#include "../../vm/execution_engine.hpp"

using namespace VM;

TEST(VM, create_instance) {

  auto fooType = new GType {
    BASICTYPES::INSTANCE, "FooType", new GType*[1] { getBoolType() }, 1
  };
  // auto instance = new GInstance { fooType, new GValue[2] {1, 2} };

  auto instructions = new GInstruction[4] {
    GInstruction { GOPCODE::INSTANCE_CREATE, new GOPARG[3] {
        // [0] = [1]([2])
        {0}, {1}, {2}
    }},
    GInstruction { END, NULL }
  };
  auto registers = new GValue[3] {
    {0}, {.asType = fooType}, { .asBool = true}
  };

  executeInstructions(NULL, instructions, registers);
  EXPECT_EQ(registers[0].asInstance->type, fooType);
  EXPECT_EQ(registers[0].asInstance->attributes[0].asBool, true);
}

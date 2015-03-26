#include <gtest/gtest.h>
#include "../../vm/execution_engine.hpp"

using namespace VM;

TEST(VM, create_instance) {

  auto fooType = new GType {
    BASICTYPES::INSTANCE, "FooType", new GType*[1] { getBoolType() },
    new std::string[0], 1
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

  GEnvironmentInstance scope {
    .locals = registers
  };

  executeInstructions(NULL, instructions, scope);
  EXPECT_EQ(fooType, registers[0].asInstance->type);
  EXPECT_EQ(true, registers[0].asInstance->attributes[0].asBool);
}

TEST(VM, test_load_attribute) {

  auto fooType = new GType {
    BASICTYPES::INSTANCE, "FooType", new GType*[1] { getBoolType() },
    new std::string[0], 1
  };
  auto instance = new GInstance { fooType, new GValue[1] {true} };

  auto instructions = new GInstruction[4] {
    GInstruction { GOPCODE::INSTANCE_LOAD_ATTRIBUTE, new GOPARG[3] {
        // [0] = [1]([2])
        {0}, {1}, {0}
    }},
    GInstruction { END, NULL }
  };
  auto registers = new GValue[2] {
    {0}, {.asInstance = instance}
  };

  GEnvironmentInstance scope {
    .locals = registers
  };

  executeInstructions(NULL, instructions, scope);
  EXPECT_EQ(true, registers[0].asBool);
}

TEST(VM, test_store_attribute) {

  auto fooType = new GType {
    BASICTYPES::INSTANCE, "FooType", new GType*[1] { getBoolType() },
    new std::string[0], 1
  };
  auto instance = new GInstance { fooType, new GValue[1] {true} };

  auto instructions = new GInstruction[4] {
    GInstruction { GOPCODE::INSTANCE_STORE_ATTRIBUTE, new GOPARG[3] {
        // [0] = [1]([2])
        {0}, {0}, {1}
    }},
    GInstruction { END, NULL }
  };
  auto registers = new GValue[2] {
    {.asInstance = instance}, {.asBool = false}
  };

  GEnvironmentInstance scope {
    .locals = registers
  };

  executeInstructions(NULL, instructions, scope);
  EXPECT_EQ(false, registers[0].asInstance->attributes[0].asBool);
}

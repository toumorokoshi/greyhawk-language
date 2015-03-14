#include <gtest/gtest.h>
#include "../../vm/vm.hpp"

using namespace VM;

// a test for a hello world execution
TEST(VM, hello_world) {

  auto modules = new GModules();

  // we have a top level function that is basically the 'main' program.
  auto scope = new G2Scope();
  scope->globals["global_value"] = new GValue {.asInt32 = 10};
  scope->addLocal("local_value");

  // auto scopeInstance = scope->createInstance();

  auto function = new GFunction {
    .returnType = getNoneType(),
    .argumentCount = 0,
    .scope = scope,
    .instructions = new GInstruction[3] {
      GInstruction { GOPCODE::GLOBAL_LOAD, new GOPARG[2] {{0}, {0}}},
      GInstruction { END, NULL }
    }
  };

  auto scopeInstance = scope->createInstance();

  function->execute(modules, scopeInstance);

  EXPECT_EQ(
}

#include <gtest/gtest.h>
#include "../../vm/vm.hpp"

using namespace VM;

// a test for a hello world execution
TEST(VM, hello_world) {

  auto modules = new GModules();

  // we have a top level function that is basically the 'main' program.
  auto scope = new G2Scope();
  scope->globals = new GValue*[1] { new GValue {.asInt32 = 10}};
  scope->globalsTable["global_value"] = 0;
  scope->globalsCount = 1;
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
  EXPECT_EQ(scopeInstance.values[0].asInt32, 10);

  // validate that, after we modify the global values,
  // executing again will modify the results.
  *scope->globals[0] = GValue{.asInt32 = 12};
  function->execute(modules, scopeInstance);
  EXPECT_EQ(scopeInstance.values[0].asInt32, 10);
}

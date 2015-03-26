#include <gtest/gtest.h>
#include "../../vm/vm.hpp"

using namespace VM;

// a test for a hello world execution
TEST(VM, hello_world_new_execution) {

  auto modules = new GModules();

  auto parentScope = new GEnvironment();
  parentScope->addObject("foo", getInt32Type());

  auto scope = parentScope->createChild();

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

  auto parentScopeInstance = parentScope->createInstance(*new GEnvironmentInstance());
  auto scopeInstance = scope.createInstance(parentScopeInstance);

  parentScopeInstance.locals[0].asInt32 = 10;
  function->execute(modules, scopeInstance);
  EXPECT_EQ(scopeInstance.locals[0].asInt32, 10);

  // validate that, after we modify the global values,
  // executing again will modify the results.
  parentScopeInstance.locals[0].asInt32 = 12;
  function->execute(modules, scopeInstance);
  EXPECT_EQ(scopeInstance.locals[0].asInt32, 12);
}

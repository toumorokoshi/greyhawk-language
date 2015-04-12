#include <gtest/gtest.h>
#include "../../vm/vm.hpp"

using namespace VM;

// a test for a hello world execution
TEST(VM, hello_world_new_execution) {

  auto modules = new GModules();

  auto parentEnvironment = new GEnvironment();
  parentEnvironment->addObject("foo", getInt32Type());

  auto environment = parentEnvironment->createChild();

  // auto environmentInstance = environment->createInstance();

  auto function = new GFunction {
    .returnType = getNoneType(),
    .argumentCount = 0,
    .environment = environment,
    .instructions = new GInstruction[3] {
      GInstruction { GOPCODE::GLOBAL_LOAD, new GOPARG[2] {{0}, {0}}},
      GInstruction { END, NULL }
    }
  };

  auto parentEnvironmentInstance = parentEnvironment->createInstance(*new GEnvironmentInstance());

  auto functionInstance = new GFunctionInstance {
    .function = function,
    .parentEnv = *parentEnvironmentInstance
  };

  parentEnvironmentInstance->locals[0].asInt32 = 10;
  // functionInstance->execute(modules);
  // EXPECT_EQ(environmentInstance->locals[0].asInt32, 10);

  // validate that, after we modify the global values,
  // executing again will modify the results.
  parentEnvironmentInstance->locals[0].asInt32 = 12;
  // functionInstance->execute(modules);
  // EXPECT_EQ(environmentInstance->locals[0].asInt32, 12);
}

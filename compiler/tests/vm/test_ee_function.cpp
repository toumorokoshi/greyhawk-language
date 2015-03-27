#include <gtest/gtest.h>
#include "../../vm/vm.hpp"

using namespace VM;

// a test for a hello world execution
TEST(VM, test_function_instantiation) {

  auto modules = new GModules();

  auto environment = new GEnvironment();
  environment->addObject("foo", getInt32Type());

  auto childEnvironment = environment->createChild();

  auto function = new GFunction {
    .returnType = getNoneType(),
    .argumentCount = 0,
    .environment = *environment,
    .instructions = new GInstruction[3] {
      GInstruction { GOPCODE::FUNCTION_CREATE, new GOPARG[2] {{0}, {.asString = "childFunction"}}},
      GInstruction { GOPCODE::FUNCTION_CALL, new GOPARG[1] {{0}}},
      GInstruction { END, NULL }
    }
  };

  auto childFunction = new GFunction {
    .returnType = getNoneType(),
    .argumentCount = 0,
    .environment = *childEnvironment,
    .instructions = new GInstruction[3] {
      GInstruction { GOPCODE::LOAD_CONSTANT_INT, new GOPARG[2] {{0}, {10}}},
      GInstruction { GOPCODE::GLOBAL_WRITE, new GOPARG[2] {{0}, {0}}},
      GInstruction { END, NULL }
    }
  };

  environment->functionTable["childFunction"] = childFunction;

  auto envInstance = environment->createInstance(*new GEnvironmentInstance());

  auto functionInstance = new GFunctionInstance {
    .function = function,
    .environmentInstance = envInstance
  };

  functionInstance->execute(modules);
  EXPECT_EQ(envInstance->locals[0].asInt32, 10);
}

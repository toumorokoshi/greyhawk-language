#include <gtest/gtest.h>
#include "../../vm/vm.hpp"
#include "../../vm/execution_engine.hpp"

using namespace VM;


TEST(VM, hello_world) {
  auto scope = new GEnvironment();
  scope->allocateObject(getNoneType());

  auto function = new GFunction {
    .returnType = getNoneType(),
    .argumentCount = 0,
    .environment = *scope,
    .instructions = new GInstruction[3] {
      GInstruction { GOPCODE::LOAD_CONSTANT_STRING, new GOPARG[2] {0, GOPARG { .asString = (char*) "hello world" }}},
      GInstruction { GOPCODE::PRINT_STRING, new GOPARG[1] { 0 } },
      GInstruction { END, NULL }
    }
  };

  executeFunction(NULL, function, *new GEnvironmentInstance(), new GValue[0]);
}

TEST(VM, for_loop) {
  auto scope = new GEnvironment();
  // temporary workraound to fill registers
  scope->allocateObject(getNoneType());
  scope->allocateObject(getNoneType());
  scope->allocateObject(getNoneType());
  scope->allocateObject(getNoneType());
  scope->allocateObject(getNoneType());

  auto function = new GFunction {
    .returnType = getNoneType(),
    .argumentCount = 0,
    .environment = *scope,
    .instructions = new GInstruction[9] {
      GInstruction { GOPCODE::LOAD_CONSTANT_INT, new GOPARG[2] { 0, 0 } },
      GInstruction { GOPCODE::LOAD_CONSTANT_INT, new GOPARG[2] { 1, 1 } },
      GInstruction { GOPCODE::LOAD_CONSTANT_INT, new GOPARG[2] { 2, 10 } },
      GInstruction { GOPCODE::LOAD_CONSTANT_STRING, new GOPARG[2] { 3, GOPARG { .asString = (char*) "hello world" }}},
      GInstruction { GOPCODE::PRINT_STRING, new GOPARG[1] { 3 } },
      GInstruction { GOPCODE::ADD_INT, new GOPARG[3] { 0, 1, 0 } },
      GInstruction { GOPCODE::LESS_THAN_INT, new GOPARG[3] { 0, 2, 4 } },
      GInstruction { GOPCODE::BRANCH, new GOPARG[3] { 4, -4, 1 } },
      GInstruction { GOPCODE::RETURN_NONE, NULL}
    },
  };

  executeFunction(NULL, function, *new GEnvironmentInstance(), new GValue[0]);

}

TEST(VM, basic_function_test) {
  auto scope = new GEnvironment();
  // temporary workraound to fill registers
  scope->allocateObject(getNoneType());
  scope->allocateObject(getNoneType());
  scope->allocateObject(getNoneType());

  auto function = new GFunction {
    .returnType = getInt32Type(),
    .argumentCount = 2,
    .environment = *scope,
    .instructions = new GInstruction[2]{
      GInstruction { GOPCODE::ADD_INT, new GOPARG[3] { 0, 1, 2 } },
      GInstruction { GOPCODE::RETURN, new GOPARG[1] { 2 }}
    },
  };

  auto arguments = new GValue[3] { 159, 73 };
  EXPECT_EQ(executeFunction(NULL, function, *new GEnvironmentInstance(), arguments).asInt32, 232);
}

/* TEST(VM, invoke_function_in_method) {


  auto function = new GOldFunction {
    getInt32Type(),
    new GInstruction[2]{
      GInstruction { GOPCODE::ADD_INT, new GOPARG[3] { 0, 1, 2 } },
      GInstruction { GOPCODE::RETURN, new GOPARG[1] { 2 }}
    },
    3,
    2
  };

  auto functionWrapper = new GOldFunction {
    getInt32Type(),
    new GInstruction[2]{
      GInstruction { GOPCODE::CALL, new GOPARG[4] { GOPARG { .function = function }, 2, 0, 1 }},
      GInstruction { GOPCODE::RETURN, new GOPARG[1] { 2 }}
    },
    3,
    2
 };



  auto arguments = new GValue[2] { 159, 73 };
  EXPECT_EQ(executeFunction(NULL, function, arguments).asInt32, 232);
  } */

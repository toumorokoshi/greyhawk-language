#include <gtest/gtest.h>
#include "../../vm/vm.hpp"
#include "../../vm/execution_engine.hpp"

using namespace VM;

TEST(vm, basic_function_test) {
  auto function = new GFunction {
    getInt32Type(),
    new GInstruction[2]{
      GInstruction { GOPCODE::ADD_INT, new GOPARG[3] { 0, 1, 2 } },
      GInstruction { GOPCODE::RETURN, new GOPARG[1] { 2 }}
    },
    3,
    2
  };

  auto arguments = new GValue[2] { 159, 73 };
  EXPECT_EQ(executeFunction(function, arguments).asInt32, 232);
}

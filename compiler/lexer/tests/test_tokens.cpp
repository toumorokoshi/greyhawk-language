#include <gtest/gtest.h>
#include "../tokens.hpp"

using namespace lexer;

TEST(OperatorPair, getOperatorPair) {
  EXPECT_EQ(&T_LPAREN, std::get<1>(T_LPAREN.getOperatorPair()));
}

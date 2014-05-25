#include <gtest/gtest.h>
#include "../utils.hpp"

using namespace lexer;

TEST(Lexer, isAlpha) {
  EXPECT_TRUE (isAlpha('t'));
}

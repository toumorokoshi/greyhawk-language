#include <gtest/gtest.h>
#include "../lexer.hpp"

using namespace lexer;

TEST(Lexer, isAlpha) {
  EXPECT_TRUE (isAlpha('t'));
}

#include <gtest/gtest.h>
#include "../lexer.hpp"

TEST(Lexer, isAlpha) {
  EXPECT_TRUE (isAlpha('t'));
}

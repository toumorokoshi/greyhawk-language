#include <gtest/gtest.h>
#include "../lexer.hpp"

using namespace lexer;

TEST(Lexer, isAlpha) {
  EXPECT_TRUE (isAlpha('t'));
}

TEST(Keyword, getDescription) {
  Keyword* kw = new Keyword("test");
  EXPECT_EQ (((Token*) kw)->getDescription(), "test");
}

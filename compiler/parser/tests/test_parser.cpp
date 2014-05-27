#include <gtest/gtest.h>
#include "../parser.hpp"

using namespace lexer;
using namespace parser;

TEST(Parser, basicTest) {
  TokenVector tokens  {
    &T_TRUE,
    &T_FALSE
  };
  parseTokens(tokens);
}

TEST(Parser, otherTest) {
  TokenVector tokens  {
    &T_TRUE,
    &T_IF
  };
  ASSERT_THROW(parseTokens(tokens), ParserException);
}

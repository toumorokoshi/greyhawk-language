#include <gtest/gtest.h>
#include "../parser.hpp"

using namespace lexer;
using namespace parser;

TEST(Parser, basicTest) {
  TokenVector tokens  {
    &T_TRUE,
    &T_FALSE
  };
  Node* node = parseTokens(parser::P2_TRUE_THEN_FALSE, tokens);
  EXPECT_EQ(node->toString(), "false");
}

TEST(Parser, otherTest) {
  TokenVector tokens  {
    &T_TRUE,
    &T_IF
  };
  ASSERT_THROW(parseTokens(parser::P2_TRUE_THEN_FALSE, tokens),
               ParserException);
}

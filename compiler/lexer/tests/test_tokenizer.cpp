#include <gtest/gtest.h>
#include "../lexer.hpp"

using namespace lexer;
using namespace std;

TEST(Tokenizer, tokenize) {
  Tokenizer tokenizer;
  TokenVector tokens = tokenizer.tokenize("== =");
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens[0], T_COMPARE_EQUAL);
  EXPECT_EQ(tokens[1], T_ASSIGN);
}

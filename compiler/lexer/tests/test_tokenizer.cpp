#include <gtest/gtest.h>
#include "../tokenizer.hpp"
#include <sstream>

using namespace lexer;
using namespace std;

TEST(Tokenizer, tokenize) {
  Tokenizer tokenizer;
  istringstream input_stream("== =");
  TokenVector tokens = tokenizer.tokenize(input_stream);
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens[0], &T_COMPARE_EQUAL);
  EXPECT_EQ(tokens[1], &T_ASSIGN);
}

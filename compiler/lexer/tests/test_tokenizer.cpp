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
  EXPECT_EQ(tokens[0]->getDescription(), T_COMPARE_EQUAL.getDescription());
  EXPECT_EQ(&T_COMPARE_EQUAL, tokens[0]);
  EXPECT_EQ(&T_ASSIGN, tokens[1]);
}

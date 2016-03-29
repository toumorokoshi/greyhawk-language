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
  EXPECT_EQ(EQUAL, tokens[0]->type);
  EXPECT_EQ(ASSIGN, tokens[1]->type);
}

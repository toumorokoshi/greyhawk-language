#include <gtest/gtest.h>
#include "../scanner.hpp"
#include <sstream>

using namespace lexer;

TEST(Scanner, validate) {
  std::istringstream input_stream("test");
  StringScanner s(input_stream);
  EXPECT_EQ(s.next(), 't');
  EXPECT_TRUE(s.hasNext());
  EXPECT_EQ(s.peek(), 'e');
  EXPECT_EQ(s.next(), 'e');
  EXPECT_EQ(s.next(), 's');
  EXPECT_EQ(s.next(), 't');
}

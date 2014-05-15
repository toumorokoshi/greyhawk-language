#include <gtest/gtest.h>
#include "../scanner.hpp"

using namespace lexer;

TEST(Scanner, validate) {
  StringScanner s("test");
  EXPECT_EQ(s.next(), 't');
  EXPECT_TRUE(s.hasNext());
  EXPECT_EQ(s.peek(), 'e');
  EXPECT_EQ(s.next(), 'e');
  EXPECT_EQ(s.next(), 's');
  EXPECT_EQ(s.next(), 't');
}

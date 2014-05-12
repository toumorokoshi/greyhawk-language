#include <gtest/gtest.h>
#include "../fsm.hpp"

using namespace lexer;

typedef FSMNode<std::string> StringNode;
typedef std::pair<std::string, std::string*> StringNodePair;
typedef std::vector<StringNodePair> StringNodePairVector;

TEST(LexerFSM, addChild) {
  std::string* result = new std::string("test");
  StringNode root = StringNode(' ', NULL);
  root.addChild("ab", result);
  EXPECT_TRUE(root.children.find('a') != root.children.end());

  EXPECT_TRUE(root.children['a'].children.find('b') !=
              root.children['a'].children.end());
}


TEST(LexerFSM, getValue) {
  std::string* result = new std::string("test");
  StringNode root = StringNode(' ', NULL);
  root.addChild("ab", result);

  EXPECT_EQ(root.getValue("ab"), result);
}


TEST(LexerFSM, addChildren) {
  std::string* foo = new std::string("foo");
  std::string* bar = new std::string("bar");
  StringNodePairVector childrenPairs {
      StringNodePair("ab", foo),
      StringNodePair("ac", bar)
  };
  StringNode root = StringNode(' ', NULL)
    .addChildren(childrenPairs);
  EXPECT_EQ(root.getValue("ab"), foo);
  EXPECT_EQ(root.getValue("ac"), bar);
}

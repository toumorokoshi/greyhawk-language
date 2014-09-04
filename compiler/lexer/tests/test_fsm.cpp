#include <gtest/gtest.h>
#include "../fsm.hpp"

using namespace lexer;

typedef std::pair<std::string, std::string*> StringNodePair;
typedef std::vector<StringNodePair> StringNodePairVector;

TEST(LexerFSM, addChild) {
  FSMNode root;
  root.addChild("ab", BAD_TOKEN);
  EXPECT_TRUE(root.children.find('a') != root.children.end());

  EXPECT_TRUE(root.children['a'].children.find('b') !=
              root.children['a'].children.end());
}


TEST(LexerFSM, getValue) {
  FSMNode root;
  root.addChild("ab", BAD_TOKEN);

  EXPECT_EQ(root.getValue("ab"), BAD_TOKEN);
}


TEST(LexerFSM, addChildren) {
  KeywordPairVector childrenPairs {
      KeywordPair("ab", IF),
      KeywordPair("ac", ELSE)
  };
  FSMNode root = FSMNode(' ', BAD_TOKEN)
    .addChildren(childrenPairs);
  EXPECT_EQ(root.getValue("ab"), IF);
  EXPECT_EQ(root.getValue("ac"), ELSE);
}

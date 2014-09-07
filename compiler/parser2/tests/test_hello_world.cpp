#include <gtest/gtest.h>
#include <string>
#include "../../vm/vm.hpp"
#include "../parser.hpp"
#include "../../lexer/tokenizer.hpp"
#include <sstream>

using namespace lexer;
using namespace parser;
using namespace VM;

TEST(Parser, helloWorld) {
  auto HELLO_WORLD = "print(\"hello world\")";
  Tokenizer tokenizer;
  std::istringstream input_stream(HELLO_WORLD);
  auto tokens = tokenizer.tokenize(input_stream);
  auto token_position = tokens.begin();
  Parser parser(&getBuiltinScope(), token_position, tokens);
  try {
    auto vm_block = parser.parseBlock();
    EXPECT_EQ(vm_block->statements.size(), 1);
    vm_block->execute();
  } catch (core::GreyhawkException e) {
    std::cout << e.message << std::endl;
  }
}

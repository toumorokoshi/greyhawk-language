#include "./parser.hpp"

using namespace lexer;

namespace parser {
  const TokenParserNode P_TRUE(lexer::T_TRUE);
  const TokenParserNode P_FALSE(lexer::T_FALSE);

  const ProductionNode P_BOOLEAN(*(new ParserNodeVector {
        &P_TRUE, &P_FALSE
  }));

  const TokenParserNode2 P2_TRUE(
    T_TRUE,
    [] () -> Node* { return new NBoolean(true); }
  );

  const TokenParserNode2 P2_FALSE(
    T_FALSE,
    [] () -> Node* { return new NBoolean(false); }
  );
}

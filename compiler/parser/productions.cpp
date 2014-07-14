#include "./parser.hpp"

using namespace lexer;

namespace parser {

  const TokenParserNode2 P2_TRUE(
    T_TRUE,
    [] () -> Node* { return new NBoolean(true); }
  );

  const TokenParserNode2 P2_FALSE(
    T_FALSE,
    [] () -> Node* { return new NBoolean(false); }
  );

  const ProductionNode2 P2_TRUE_THEN_FALSE (
    *(new ParserNodeVector2{ &P2_TRUE, &P2_FALSE }),
    [] (std::vector<Node*>& nodes) -> Node* {
      return new NBoolean(false);
    }
  );
}

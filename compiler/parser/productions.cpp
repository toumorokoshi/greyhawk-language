#include "./parser.hpp"

using namespace lexer;

namespace parser {

  const TokenProduction P2_TRUE(
    T_TRUE,
    [] () -> Node* { return new NBoolean(true); }
  );

  const TokenProduction P2_FALSE(
    T_FALSE,
    [] () -> Node* { return new NBoolean(false); }
  );

  const SeriesProduction P2_TRUE_THEN_FALSE (
    *(new ProductionVector{ &P2_TRUE, &P2_FALSE }),
    [] (std::vector<Node*>& nodes) -> Node* {
      return new NBoolean(false);
    }
  );
}

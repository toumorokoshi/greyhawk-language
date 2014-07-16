#include "../lexer/tokens.hpp"
#include "../node.hpp"
#include "./exceptions.hpp"
#include <functional>

#ifndef PARSER_HPP
#define PARSER_HPP

namespace parser {

  class Production {
  public:
    Production() {}
    virtual ~Production() {}
    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position) const = 0;
  };

  typedef std::vector<const Production*> ProductionVector;

  class TokenProduction: public Production {

    const lexer::Token& token;
    const std::function<Node* ()> generateNode;

  public:
    TokenProduction (
        const lexer::Token& _token,
        const std::function<Node* ()> _generateNode
    ): token(_token), generateNode(_generateNode) {}

    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position) const;
  };

  class SeriesProduction: public Production {
    const ProductionVector& _parserNodes;
    const std::function<Node* (std::vector<Node*>&)> _generateNode;
  public:

    SeriesProduction(
        ProductionVector& parserNodes,
        std::function<Node* (std::vector<Node*>&)> generateNode
    ) :
      _parserNodes(parserNodes),
      _generateNode(generateNode) {}

    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position) const;
  };

  Node& parseTokens(const Production& root, lexer::TokenVector& tokens);

  extern const TokenProduction P2_TRUE;
  extern const SeriesProduction P2_TRUE_THEN_FALSE;
}

#endif

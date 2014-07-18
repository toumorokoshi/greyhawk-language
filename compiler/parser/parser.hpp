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
    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position,
                              lexer::TokenVector::iterator token_end) const = 0;
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

    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position,
                              lexer::TokenVector::iterator token_end) const;
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

    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position,
                              lexer::TokenVector::iterator token_end) const;
  };

  // production for collecting multiple of a particular type of statement.
  class ListProduction: public Production {
    const Production& _production;
    const std::function<Node* (std::vector<Node*>&)> _generateNode;
  public:

    ListProduction(
        Production production,
        std::function<Node* (std::vector<Node*>&)> generateNode
    ) :
      _production(production),
      _generateNode(generateNode) {}

    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position,
                              lexer::TokenVector::iterator token_end) const;
  };

  // with the vector of production to match, the first match will be returned.
  class ManyProduction: public Production {
    const ProductionVector& _productions;

  public:
    ManyProduction(ProductionVector& productions) : _productions(productions) {}
    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position,
                              lexer::TokenVector::iterator token_end) const;
  };

  Node* parseTokens(const Production& root, lexer::TokenVector& tokens);

  extern const TokenProduction P2_TRUE;
  extern const SeriesProduction P2_TRUE_THEN_FALSE;

  NStatement* parseStatement(lexer::TokenVector::iterator& token_position,
                            lexer::TokenVector::iterator token_end);

  NExpression* parseExpression(lexer::TokenVector::iterator& token_position,
                               lexer::TokenVector::iterator token_end);

  NExpression* parseNumeric(lexer::TokenVector::iterator& token_position,
                            lexer::TokenVector::iterator token_end);

  NExpression* parseSingleNumeric(lexer::TokenVector::iterator& token_position,
                                  lexer::TokenVector::iterator token_end);
}

#endif

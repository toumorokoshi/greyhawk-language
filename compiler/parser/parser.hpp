#include "../lexer/tokens.hpp"
#include "../node.hpp"
#include "./exceptions.hpp"
#include <functional>

#ifndef PARSER_HPP
#define PARSER_HPP

namespace parser {

  class ParserNode2 {
  public:
    ParserNode2() {}
    virtual ~ParserNode2() {}
    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position) const = 0;
  };

  typedef std::vector<const ParserNode2*> ParserNodeVector2;

  class TokenParserNode2: public ParserNode2 {

    const lexer::Token& token;
    const std::function<Node* ()> generateNode;

  public:
    TokenParserNode2(
        const lexer::Token& _token,
        const std::function<Node* ()> _generateNode
    ): token(_token), generateNode(_generateNode) {}

    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position) const;
  };

  class ProductionNode2: public ParserNode2 {
    const ParserNodeVector2& _parserNodes;
    const std::function<Node* (std::vector<Node*>&)> _generateNode;
  public:
    ProductionNode2(ParserNodeVector2& parserNodes,
                    std::function<Node* (std::vector<Node*>&)> generateNode) :
      _parserNodes(parserNodes),
      _generateNode(generateNode) {}
    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position) const;
  };

  Node& parseTokens2(const ParserNode2& root, lexer::TokenVector& tokens);

  extern const TokenParserNode2 P2_TRUE;
  extern const ProductionNode2 P2_TRUE_THEN_FALSE;
}

#endif

#include "../lexer/tokens.hpp"
#include "../node.hpp"
#include "./exceptions.hpp"
#include <functional>

#ifndef PARSER_HPP
#define PARSER_HPP

namespace parser {

  // abstract base class
  class ParserNode {
  public:
    typedef std::vector<const ParserNode*> ParserNodeVector;
    ParserNode() {}
    virtual ~ParserNode() {}
    virtual Node& generateCompilerNode(NodeVector& nodeVector) const = 0;
    virtual bool isTerminal() const { return false; }
    virtual const ParserNodeVector& getParseNodeVector() const { return *(new ParserNodeVector()); }
    virtual const lexer::Token& getMatchingToken() const {
      throw ParserException("ParserNode does not have a token representation!");}
  };

  typedef ParserNode::ParserNodeVector ParserNodeVector;

  class TokenParserNode: public ParserNode {
  public:
    const lexer::Token& token;
    // const std::function<Node* (Token&)> resolver;
    TokenParserNode(const lexer::Token& _token) : token(_token) {}
    bool isTerminal() const { return true; }
    virtual Node& generateCompilerNode(NodeVector& nodeVector) const {
      return *(new NBoolean(true));
    }
    virtual const lexer::Token& getMatchingToken() const { return token; }
  };

  class ProductionNode: public ParserNode {
  public:
    const ParserNodeVector& children;
    ProductionNode(ParserNodeVector& _children) : children(_children) {}
    virtual const ParserNodeVector& getParseNodeVector() const { return children; }
    // TODO: add actual stuff in
    virtual Node& generateCompilerNode(NodeVector& nodeVector) const {
      return *(new NBoolean(true));
    }
  };

  Node& parseTokens(lexer::TokenVector& tokens);
  Node* matchNode(const ParserNode* node, lexer::TokenVector::iterator& token_position);

  // the actual parse tree
  const ProductionNode parserRootNode(*(new ParserNodeVector {
          new TokenParserNode(lexer::T_TRUE),
          new TokenParserNode(lexer::T_FALSE)
            }));
}

#endif

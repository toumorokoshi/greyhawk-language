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
    const ParserNodeVector2 _parserNodes;
    const std::function<Node* ()> _generateNode;
  public:
    ProductionNode2(ParserNodeVector2 parserNodes) :
      _parserNodes(parserNodes) {}
    virtual Node* parseTokens(lexer::TokenVector::iterator& token_position) const;
  };

  Node& parseTokens2(const ParserNode2& root, lexer::TokenVector& tokens);

  // abstract base class
  class ParserNode {
  public:
    // the parser node vector is the a list of
    // parser node tokens which must be recursively matched
    // to produce a proper tree
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

  /*
    This represents a single token, wrapped in a parser to fit with the parser types
   */
  class TokenParserNode: public ParserNode {
  public:
    const lexer::Token& token;
    // const std::function<Node* (Token&)> resolver;
    TokenParserNode(const lexer::Token& _token) : token(_token) {}
    // a terminal node is the lowest level node in the tree
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

  const ProductionNode& getParserRootNode();

  extern const TokenParserNode P_TRUE;
  extern const TokenParserNode P_FALSE;
  extern const ProductionNode P_BOOLEAN;

  extern const TokenParserNode2 P2_TRUE;
}

#endif

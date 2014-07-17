#include "./parser.hpp"
#include <stdio.h>

using namespace lexer;

namespace parser {

  Node* parseTokens(const Production& root, TokenVector& tokens) {
    auto tokens_head = tokens.begin();
    auto rootNode = root.parseTokens(tokens_head, tokens.end());
    if (rootNode == NULL) {
      throw ParserException("No root node generated!");
    } else if (tokens_head != tokens.end()) {
      while (tokens_head != tokens.end()) {
        std::cout << (*tokens_head)->getDescription() << std::endl;
        tokens_head++;
      }
      throw ParserException("Not all tokens were consumed!!");
    }
    return rootNode;
  }

  Node* TokenProduction::parseTokens(TokenVector::iterator& token_position,
                                      TokenVector::iterator token_end) const {
    if (*token_position == &(this->token)) {
      token_position++;
      return this->generateNode();
    }
    return NULL;
  }

  Node* SeriesProduction::parseTokens(TokenVector::iterator& token_position,
                                      TokenVector::iterator token_end) const {
    auto start_token_position = token_position;
    std::vector<Node*> nodes;
    for (auto production = _parserNodes.begin(); production != _parserNodes.end(); production++) {
      Node* node = (*production)->parseTokens(token_position, token_end);

      bool invalid = (
        node == NULL ||
        (token_position == token_end && (production + 1) != _parserNodes.end())
      );

      if (invalid) {
        token_position = start_token_position;
        return NULL;
      }

      nodes.push_back(node);
    }
    return _generateNode(nodes);
  }

  Node* ListProduction::parseTokens(TokenVector::iterator& token_position,
                                    TokenVector::iterator token_end) const {
    auto start_token_position = token_position;
    std::vector<Node*> nodes;
    while(token_position != token_end) {
      Node* node = _production.parseTokens(token_position, token_end);
      if (node == NULL) {
        token_position = start_token_position;
        return NULL;
      }
      nodes.push_back(node);
    }
    return _generateNode(nodes);
  }

  Node* ManyProduction::parseTokens(TokenVector::iterator& token_position,
                                    TokenVector::iterator token_end) const {
    auto start_token_position = token_position;
    for (auto production : _productions) {
      Node* node = production->parseTokens(token_position, token_end);
      if (node != NULL) {
        return node;
      }
      token_position = start_token_position;
    }
    return NULL;
  }
}

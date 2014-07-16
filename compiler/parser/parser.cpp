#include "./parser.hpp"
#include <stdio.h>

using namespace lexer;

namespace parser {

  /* THE NEW STUFF  */
  Node& parseTokens(const Production& root, TokenVector& tokens) {
    auto tokens_head = tokens.begin();
    std::cout << (*tokens_head)->getDescription() << std::endl;
    auto rootNode = root.parseTokens(tokens_head);
    if (rootNode == NULL) {
      throw ParserException("No root node generated!");
    } else if (tokens_head != tokens.end()) {
      while (tokens_head != tokens.end()) {
        std::cout << (*tokens_head)->getDescription() << std::endl;
        tokens_head++;
      }
      throw ParserException("Not all tokens were consumed!!");
    }
    return *rootNode;
  }

  Node* TokenProduction::parseTokens(TokenVector::iterator& token_position) const {
    if (*token_position == &(this->token)) {
      token_position++;
      return this->generateNode();
    }
    return NULL;
  }

  Node* SeriesProduction::parseTokens(TokenVector::iterator& token_position) const {
    std::vector<Node*> nodes;
    for (auto i : _parserNodes) {
      nodes.push_back(i->parseTokens(token_position));
    }
    return _generateNode(nodes);
  }
}

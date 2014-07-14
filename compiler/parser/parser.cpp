#include "./parser.hpp"
#include <stdio.h>

using namespace lexer;

namespace parser {

  /* THE NEW STUFF  */
  Node& parseTokens2(const ParserNode2& root, TokenVector& tokens) {
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

  Node* TokenParserNode2::parseTokens(TokenVector::iterator& token_position) const {
    if (*token_position == &(this->token)) {
      token_position++;
      return this->generateNode();
    }
    return NULL;
  }

  Node* ProductionNode2::parseTokens(TokenVector::iterator& token_position) const {
    for (auto i : _parserNodes) {
    }
  }

  /* END THE NEW STUFF  */

  const ProductionNode& getParserRootNode() {
    // the actual parse tree
    return P_BOOLEAN;
  }

  // I don't know what I'm doing here, just going to see if I can start
  // writing the algorithm I want

  Node& parseTokens(TokenVector& tokens) {
    TokenVector::iterator head = tokens.begin();
    const ParserNode* root = &getParserRootNode();
    Node* rootNode = matchNode(root, head);
    if (rootNode == NULL) {
      throw ParserException("No root node generated!");
    }
    return *rootNode;
  }

  /*
    Matches a node with the token vector, retrieving tokens until it is satisifed, or returning null if
    the token vector does not match
  */
  Node* matchNode(const ParserNode* node, TokenVector::iterator& token_position) {
    NodeVector nodeVector;

    if (node->isTerminal()) {
      const Token& matchingToken = node->getMatchingToken();

      if(*token_position == &matchingToken) {
        // we just populate with a blank node for now
        nodeVector.push_back(new Node());
        token_position++;

      } else {
        return NULL;

      }

    } else {

      for (const ParserNode* parserNode: node->getParseNodeVector()) {
        Node* innerNode = matchNode(parserNode, token_position);

        if (innerNode == NULL) {
          return NULL;
        }
        nodeVector.push_back(innerNode);
      }
    }
    return &(node->generateCompilerNode(nodeVector));
  }

}

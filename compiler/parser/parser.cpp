#include "./parser.hpp"


using namespace parser;
using namespace lexer;

// I don't know what I'm doing here, just going to see if I can start
// writing the algorithm I want

Node& parser::parseTokens(TokenVector& tokens) {
  TokenVector::iterator head = tokens.begin();
  const ParserNode* root = &parserRootNode;
  Node* rootNode = matchNode(root, head);
  if (rootNode == NULL) {
    throw ParserException("No root node generated!");
  }
  return *rootNode;
}

Node* parser::matchNode(const ParserNode* node, lexer::TokenVector::iterator& token_position) {
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
      nodeVector.push_back(matchNode(parserNode, token_position));
      token_position++;
    }
  }
  return &(node->generateCompilerNode(nodeVector));
}

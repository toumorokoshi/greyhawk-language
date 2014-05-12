// put tokenizer methods in here
#include "lexer.hpp"

using namespace lexer;
using std::string;



Tokenizer::Tokenizer() {
}

TokenVector Tokenizer::tokenize(std::string input) {
    initialize();

    for (char& c : input) {
      if (c == ' ') {
        flushKeyword();
        flushOperator();
        continue;
      }

      if (isAlphaNumeric(c)) {

        current_token += c;
        matchKeyword();

      } else {

        flushKeyword();
        traverseOperatorFSM(c);

      }
    }
    flushKeyword();
    flushOperator();
    return *tokens;
};

void Tokenizer::flushKeyword() {
  if (current_token.compare("") != 0) {
    tokens->push_back(new Identifier(current_token));
    current_token = "";
  }
};

void Tokenizer::flushOperator() {
  if (current_node->value == NULL && current_node != operator_root) {
    throw LexerException("Unable to find token matching " + string(1, current_node->token));
  }
  if (current_node->value != NULL) {
    tokens->push_back(current_node->value);
  }
}

void Tokenizer::initialize() {
  tokens = new TokenVector();
  operator_root = &operatorFSM;
  current_node = &operatorFSM;
  current_token = "";
}

void Tokenizer::traverseOperatorFSM(char c) {

  if (current_node->hasChild(c)) {
    current_node = &(current_node->children[c]);
  } else {

    if (current_node->value == NULL) {
      throw LexerException("Unable to find token matching " + string(1, c));

    } else {
      tokens->push_back(current_node->value);
      current_node = operator_root;
    }
  }

}

void Tokenizer::matchKeyword() {
  for (KeywordVector::iterator it = keywordList.begin(); it != keywordList.end(); ++it) {
    if (current_token.compare((*it)->symbol) == 0) {
      current_token = "";
      tokens->push_back(*it);
    }
  }
}

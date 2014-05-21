// put tokenizer methods in here
#include "lexer.hpp"

using namespace lexer;
using std::string;

TokenVector Tokenizer::tokenize(std::string input) {
  StringScanner scanner(input);
  TokenVector tokens;

  while (scanner.hasNext()) {
    if (scanner.peek() == ' ') {
      // we don't care about whitespace
      scanner.next();

    } else if (isAlphaNumeric(scanner.peek())) {
      // if the next character is alphanumeric,
      // we pass it to the keyword matcher
      tokens.push_back(&matchKeyword(scanner));

    } else {
      // if it's not, we pass it to our operatorFSM
      tokens.push_back(&matchOperator(scanner));
    }
  }
  return tokens;
};

const Token& Tokenizer::matchOperator(StringScanner& scanner) {
  OperatorFSM* current_node = &operatorFSM;

  while (scanner.hasNext() && current_node->hasChild(scanner.peek())) {
    current_node = &(current_node->children[scanner.next()]);
  }

  if (current_node->value == NULL) {

    if (scanner.hasNext()) {
      throw LexerException("Unable to find token matching " + string(1, scanner.peek()));
    } else {
      throw LexerException("Unable to find matching operator sequence");
    }

  } else {
    return *(current_node->value);

  }
}

const Token& Tokenizer::matchKeyword(StringScanner& scanner) {
  std::string current_token("");

  while (scanner.hasNext() && scanner.peek() != ' ') {
    current_token += scanner.next();
  }

  for (KeywordVector::const_iterator it = keywordList.begin(); it != keywordList.end(); ++it) {
    if (current_token.compare((*it).symbol) == 0) {
      return *it;
    }
  }

  return *(new Identifier(current_token));
}

// put tokenizer methods in here
#include "tokenizer.hpp"

using namespace lexer;
using std::string;

OperatorFSM& lexer::getOperatorFSMRoot() {
  static OperatorFSM root(' ', NULL);
  static bool initialized = false;
  if (!initialized) {
    initialized = true;
    root.addChildren(operatorPairs);
  }
  return root;
}

TokenVector Tokenizer::tokenize(std::istream& input) {
  StringScanner scanner(input);
  TokenVector tokens;
  bool isNewLine = true;
  initialize();

  while (scanner.hasNext()) {
    if (isNewLine) {
      isNewLine = false;
      calculateIndent(scanner, tokens);

    } else if (scanner.peek() == ' ') {
      // we don't care about whitespace
      scanner.next();

    } else if (scanner.peek() == '\n') {
      isNewLine = true;
      scanner.next();

    } else if (isNumeric(scanner.peek())) {
      // then it's a number
      tokens.push_back(&matchNumber(scanner));

    } else if (isAlpha(scanner.peek())) {
      // if the next character is alphanumeric,
      // we pass it to the keyword matcher
      tokens.push_back(&matchKeyword(scanner));

    } else {
      // if it's not, we pass it to our operatorFSM
      tokens.push_back(&matchOperator(scanner));
    }
  }
  clearIndent(tokens);
  return tokens;
};

void Tokenizer::initialize() {
  indentation = 0;
}

const Token& Tokenizer::matchOperator(StringScanner& scanner) {
  OperatorFSM* current_node = &getOperatorFSMRoot();

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
  string current_token("");

  while (scanner.hasNext()) {
    char next = scanner.peek();

    if (isTokenBreakCharacter(next) || !isAlphaNumeric(next)) {
      break;
    }

    current_token += scanner.next();
  }

  for (KeywordVector::const_iterator it = keywordList.begin(); it != keywordList.end(); ++it) {
    if (current_token.compare((*it)->symbol) == 0) {
      return **it;
    }
  }

  return *(new Identifier(current_token));
}

const Token& Tokenizer::matchNumber(StringScanner& scanner) {
  string current_token("");
  bool isDouble = false;
  while(scanner.hasNext()) {
    char c = scanner.peek();

    if (c == '.') {
      if (isDouble) {
        break;
      }
      isDouble = true;

    } else if (!isNumeric(c)) {
      break;

    }
    current_token += c;
    scanner.next();
  }

  if (isDouble) {
    return *(new Double(stod(current_token)));
  } else {
    return *(new Integer(stoi(current_token)));
  }
}


void Tokenizer::calculateIndent(StringScanner& scanner, TokenVector& tokens) {

  int current_indentation = 0;
  while (scanner.hasNext() && scanner.peek() == '\t') {
    current_indentation++;
    scanner.next();
  }

  while (indentation > current_indentation) {
    indentation--;
    tokens.push_back(&T_UNINDENT);
  }

  while (indentation < current_indentation) {
    indentation++;
    tokens.push_back(&T_INDENT);
  }
}

void Tokenizer::clearIndent(TokenVector& tokens) {
  while(indentation > 0) {
    indentation--;
    tokens.push_back(&T_UNINDENT);
  }
}

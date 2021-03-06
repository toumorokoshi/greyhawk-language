// put tokenizer methods in here
#include "../spec.hpp"
#include "tokenizer.hpp"
#include <sstream>

using namespace lexer;
using std::string;

FSMNode& lexer::getOperatorFSMRoot() {
  static FSMNode root(' ', BAD_TOKEN);
  static bool initialized = false;
  if (!initialized) {
    initialized = true;
    root.addChildren(operatorPairs);
  }
  return root;
}

TokenVector Tokenizer::tokenize(std::istream& input) {
  int line = 0;
  StringScanner scanner(input);
  TokenVector tokens;
  bool isNewLine = true;
  bool isComment = false;
  initialize();

  while (scanner.hasNext()) {

    if (isNewLine) {
      /* when we see an empty line, (newline followed by newline)
         it doesn't count toward the indentation calculation.
         this allows empty lines to be used for code organization:

         if x:
           x = "test"

           x = "not test"
      */
      while(scanner.peek() == '\n') {
        scanner.next();
        line++;
      }

      line++;
      isNewLine = false;
      isComment = false;
      calculateIndent(scanner, tokens, line);


    } else if (scanner.peek() == ' ') {
      // we don't care about whitespace
      scanner.next();

    } else if (scanner.peek() == '\n') {
      isNewLine = true;
      scanner.next();

    } else if (isComment) {
      scanner.next();

    } else if (scanner.peek() == '/') {
      scanner.next();
      if (scanner.peek() == '/') {
        scanner.next();
        isComment = true;
      } else {
        scanner.back();
        tokens.push_back(&matchOperator(scanner, line));
      }

    } else if (scanner.peek() == '\'') {
      char* values = new char[1];
      scanner.next();
      values[0] = parseChar(scanner, line);
      if (scanner.peek() != '\'') {
        throw LexerException(line, "expected a single quote, to close a character definition", "");
      }
      scanner.next();
      tokens.push_back(new Token(CHAR, line, values));

    } else if (scanner.peek() == '"') {
      std::string output;
      scanner.next();
      while(scanner.peek() != '"') {
        output.push_back(parseChar(scanner, line));
      }
      scanner.next();
      tokens.push_back(new Token(STRING, line, output));

    } else if (isNumeric(scanner.peek())) {
      // then it's a number
      tokens.push_back(&matchNumber(scanner, line));

    } else if (isAlpha(scanner.peek())) {
      // if the next character is alphanumeric,
      // we pass it to the keyword matcher
      tokens.push_back(&matchKeyword(scanner, line));

    } else {
      // if it's not, we pass it to our operatorFSM
      tokens.push_back(&matchOperator(scanner, line));
    }
  }
  clearIndent(tokens, line);
  return tokens;
};

void Tokenizer::initialize() {
  indentation = 0;
}

char Tokenizer::parseChar(StringScanner& scanner, int line) {
  if (scanner.peek() != '\\') {
    return scanner.next();
  }
  scanner.next();
  switch (scanner.peek()) {
  case '0':
    scanner.next();
    return EOF;
  case '\'':
    scanner.next();
    return '\'';
  case '"':
    scanner.next();
    return '"';
  default:
    throw LexerException(line, "unable to escape character", "");
  }
}

const Token& Tokenizer::matchOperator(StringScanner& scanner, int line) {
  FSMNode* current_node = &getOperatorFSMRoot();

  while (scanner.hasNext() && current_node->hasChild(scanner.peek())) {
    current_node = &(current_node->children[scanner.next()]);
  }

  if (current_node->value == BAD_TOKEN) {

    if (scanner.hasNext()) {
      throw LexerException(line,
                           "Unable to find token matching " + string(1, scanner.peek()),
                           "");
    } else {
      throw LexerException(line,
                           "Unable to find matching operator sequence",
                           "");
    }

  } else {
    return *new Token(current_node->value, line);
  }
}

const Token& Tokenizer::matchKeyword(StringScanner& scanner, int line) {
  string current_token("");
  bool startsWithCapital = false;
  bool containsNonStartingCapital = false;

  char firstToken = scanner.next();
  if (isCapital(firstToken)) {
    startsWithCapital = true;
  }

  current_token += firstToken;

  while (scanner.hasNext()) {
    char next = scanner.peek();

    if (isTokenBreakCharacter(next) || !isAlphaNumeric(next)) {
      break;
    }

    if (isCapital(next)) {
      containsNonStartingCapital = true;
    }
    current_token += scanner.next();
  }

  for (KeywordPairVector::const_iterator it = keywordList.begin(); it != keywordList.end(); ++it) {
    if (current_token.compare(it->first) == 0) {
      return *new Token(it->second, line);
    }
  }

  if (startsWithCapital) {
    return *new Token(TYPE, line, current_token);
  }

  return *new Token(IDENTIFIER, line, current_token);
}

const Token& Tokenizer::matchNumber(StringScanner& scanner, int line) {
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
    return *(new Token(DOUBLE, line, current_token));
  } else {
    return *(new Token(INT, line, current_token));
  }
}


void Tokenizer::calculateIndent(StringScanner& scanner, TokenVector& tokens, int line) {

  int current_indentation = 0;
  while (scanner.hasNext() && scanner.peek() == '\t') {
    current_indentation++;
    scanner.next();
  }

  while (indentation > current_indentation) {
    indentation--;
    tokens.push_back(new Token(UNINDENT, line));
  }

  while (indentation < current_indentation) {
    indentation++;
    tokens.push_back(new Token(INDENT, line));
  }

  if (scanner.peek() == ' ') {
    throw LexerException(line,
                         "Spaces cannot be at the beginning of a line!",
                         spec::TABS_SCOPE);
  }
}

void Tokenizer::clearIndent(TokenVector& tokens, int line) {
  while(indentation > 0) {
    indentation--;
    tokens.push_back(new Token(UNINDENT, line));
  }
}

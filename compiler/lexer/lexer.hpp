#include <string>
#include <exception>
#include <boost/regex.hpp>

#ifndef LEXER_HPP

#define LEXER_HPP

using namespace std;

// extend exceptions from here if they have to do with
// the lexer specifically
class LexerException: public exception {
public:
  const string message;
  LexerException(string _message) : message(_message) {}
};

class Token {
public:
  virtual string getDescription() { return "nothing"; }
};


class Keyword : public Token {
public:
  const string symbol;
  Keyword(string _symbol) : symbol(_symbol) {}
  string getDescription() { return symbol; }
};


typedef vector<Token> TokenVector;
typedef vector<Keyword> KeywordVector;

TokenVector tokenize(string input);

// utility methods
bool isAlpha(char);
bool isNumeric(char);
bool isAlphaNumeric(char);

#endif

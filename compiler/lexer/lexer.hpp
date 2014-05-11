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
  const string name;
  const boost::regex pattern;
  const bool ignore;
  Token(string _name, string _pattern, bool ignore)
    : name(_name),
      pattern(boost::regex("^" + _pattern, boost::regex::ECMAScript)),
      ignore(ignore) {}
  Token(string _name, string _pattern) :
    Token(_name, _pattern, false) {}
};

typedef vector<Token> TokenVector;

// defined in lexer.cpp
extern Token T_FOO;
extern Token T_BAR;
extern Token T_NOTHING;
extern TokenVector tokensByPrecedence;

TokenVector tokenize(string input);

#endif

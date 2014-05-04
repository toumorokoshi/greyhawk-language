#include <string>
#include <exception>
#include <boost/regex.hpp>

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

Token T_FOO = Token("foo", "testing");
Token T_BAR = Token("bar", "none");
Token T_NOTHING = Token("null", " ", true);

TokenVector tokensByPrecedence {
  T_FOO,
    T_BAR,
    T_NOTHING
};

TokenVector tokenize(string input);

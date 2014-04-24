#include <string>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

class Token {

public:
  const string name;
  const regex pattern;
  const bool ignore;
  Token(string _name, string _pattern, bool ignore)
    : name(_name),
      pattern(regex("^" + _pattern, regex::ECMAScript)),
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

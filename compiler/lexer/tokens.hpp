#include <string>
#include <vector>
#include <map>

#ifndef LEXER_TOKEN_HPP
#define LEXER_TOKEN_HPP

namespace lexer {

  // NOTE: all tokens should be initialized as const
  // Tokens are (should be) immutable and should be treated as such

  // this is a list of actual tokens used
  // helpful for parsers
  enum L {
    // generic token, this means an error
    BAD_TOKEN,
    // base types
    INT,
    DOUBLE,
    STRING,
    IDENTIFIER,
    TYPE,
    // actual tokens
    IF,
    ELSE,
    RETURN,
    TRUE,
    FALSE,
    TYPE_TOKEN,
    INDENT,
    UNINDENT,
    LPAREN,
    RPAREN,
    COMMA,
    DOT,
    COLON,
    DECLARE,
    ASSIGN,
    L_BRACKET,
    R_BRACKET,
    // binary operators
    PLUS,
    MINUS,
    MUL,
    DIV,
    EQUAL,
    NOT_EQUAL,
    LESS_OR_EQUAL,
    GREATER_OR_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    IS
    // end binary ops
  };

  extern std::map<L, std::string> tokenMap;

  class Token {
  public:
    const L type;
    const int line;
    const std::string value;
    Token(L _type, int _line) : Token(_type, _line, "") {}
    Token(L _type, int _line, std::string _value) :
      type(_type), line(_line), value(_value)  {}
    virtual ~Token() {}

    virtual std::string getDescription() const {
      auto output_string = tokenMap.find(type) != tokenMap.begin() ? tokenMap[type] : "token";
      if (value != "") {
        output_string += ": " + value;
      }
      return output_string;
    }

    virtual std::string getFullDescription() const {
      return "line " + std::to_string(line) + ": " + getDescription();
    }

  };

  typedef std::vector<const Token*> TokenVector;

  typedef std::pair<std::string, L> KeywordPair;
  typedef std::vector<KeywordPair> KeywordPairVector;

  TokenVector tokenize(std::string input);

  // Grouping the tokens up
  extern const KeywordPairVector keywordList;
  extern const KeywordPairVector operatorPairs;
}

#endif

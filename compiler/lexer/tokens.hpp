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
    INDENT,
    UNINDENT,
    LPAREN,
    RPAREN,
    COMMA,
    DOT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    DECLARE,
    ASSIGN,
    EQUAL,
    NOT_EQUAL,
    LESS_OR_EQUAL,
    GREATER_OR_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    COLON,
    IS
  };

  extern std::map<L, std::string> tokenMap;

  class Token {
  public:
    const L type;
    const int line;
    Token(L _type, int _line) : type(_type), line(_line)  {}
    virtual ~Token() {}
    virtual std::string getDescription() const {
      return tokenMap.find(type) != tokenMap.begin() ? tokenMap[type] : "token";
    }
    virtual std::string getFullDescription() const {
      return "line " + std::to_string(line) + ": " + getDescription();
    }
    // returns true if it's a terminal token (this is for the parser)
    virtual bool isTerminal() { return true; }
  };

  class Integer : public Token {
  public:
    const int value;
    Integer(int line, int _value): Token(INT, line), value(_value) { }
    std::string getDescription() const { return "integer: " + std::to_string(value); }
  };

  class Double : public Token {
  public:
    const double value;
    Double(int line, double _value): Token(DOUBLE, line), value(_value) {}
    std::string getDescription() const { return "double: " + std::to_string(value); }
  };

  class String: public Token {
  public:
    const std::string value;
    String(int line, std::string _value) : Token(STRING, line), value(_value) {}
    std::string getDescription() const { return "string: \"" + value + "\""; }
  };

  class Identifier : public Token {
  public:
    const std::string name;
    Identifier(int line, std::string _name) : Token(IDENTIFIER, line), name(_name) {}
    std::string getDescription() const { return "identifier: " + name; }
  };

  class TypeToken: public Token {
  public:
    const std::string name;
    TypeToken(int line, std::string _name) : Token(TYPE, line), name(_name) {}
    std::string getDescription() const { return "type: " + name; }
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

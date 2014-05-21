#include <string>
#include <vector>

#ifndef LEXER_TOKEN_HPP
#define LEXER_TOKEN_HPP

namespace lexer {

  // NOTE: all tokens should be initialized as const
  // Tokens are (should be) immutable and should be treated as such

  class Token {
  private:
    std::string name;
  public:
    Token() : name("nothing") {}
    Token(std::string _name) : name(_name) {}
    virtual std::string getDescription() const { return "token: " + name; }
  };

  class Operator :  public Token {
    typedef std::pair<std::string, const Operator*> OperatorPair;
  public:
    const std::string symbols;
    Operator(std::string _symbols) : symbols(_symbols) {};
    std::string getDescription() const { return symbols; }
    OperatorPair getOperatorPair () const { return OperatorPair(symbols, this); }
  };

  class Keyword : public Token {
  public:
    const std::string symbol;
    Keyword(std::string _symbol) : symbol(_symbol) {}
    std::string getDescription() const { return "keyword: " + symbol; }
  };

  class Integer : public Token {
  public:
    const int value;
    Integer(int _value): value(_value) {}
    std::string getDescription() const { return "integer: " + std::to_string(value); }
  };

  class Double : public Token {
  public:
    const double value;
    Double(double _value): value(_value) {}
    std::string getDescription() const { return "double: " + std::to_string(value); }
  };

  class Identifier : public Token {
  public:
    const std::string name;
    Identifier(std::string _name) : name(_name) {}
    std::string getDescription() const { return "identifier: " + name; }
  };

  typedef std::vector<const Token*> TokenVector;
  typedef std::vector<Keyword> KeywordVector;

  typedef std::pair<std::string, const Operator*> OperatorPair;
  typedef std::vector<OperatorPair> OperatorPairVector;

  TokenVector tokenize(std::string input);

  // the actual tokens
  const Token T_INDENT("indent");
  const Token T_UNINDENT("unindent");

  // keywords
  const Keyword T_ELSE("else");
  const Keyword T_IF("if");
  const Keyword T_IS("is");
  const Keyword T_RETURN("return");

  // constants
  const Keyword T_TRUE("true");
  const Keyword T_FALSE("false");

  // operators
  const Operator T_LPAREN("(");
  const Operator T_RPAREN(")");
  const Operator T_COMMA(",");
  const Operator T_DOT(".");
  const Operator T_PLUS("+");
  const Operator T_MINUS("-");
  const Operator T_MUL("*");
  const Operator T_DIV("/");
  const Operator T_DECLARE(":=");
  const Operator T_ASSIGN("=");
  const Operator T_COMPARE_EQUAL("==");
  const Operator T_COMPARE_NOT_EQUAL("!=");
  const Operator T_COMPARE_LEQ("<=");
  const Operator T_COMPARE_GEQ(">=");
  const Operator T_COMPARE_LET("<");
  const Operator T_COMPARE_GET(">");
  const Operator T_COLON(":");

  // Grouping the tokens up
  const KeywordVector keywordList{
      T_ELSE,
      T_IF,
      T_IS,
      T_RETURN,
      T_FALSE,
      T_TRUE
  };

  const OperatorPairVector operatorPairs {
    T_LPAREN.getOperatorPair(),
    T_RPAREN.getOperatorPair(),
    T_COMMA.getOperatorPair(),
    T_DOT.getOperatorPair(),
    T_PLUS.getOperatorPair(),
    T_MINUS.getOperatorPair(),
    T_MUL.getOperatorPair(),
    T_DIV.getOperatorPair(),
    T_DECLARE.getOperatorPair(),
    T_ASSIGN.getOperatorPair(),
    T_COMPARE_EQUAL.getOperatorPair(),
    T_COMPARE_NOT_EQUAL.getOperatorPair(),
    T_COMPARE_LEQ.getOperatorPair(),
    T_COMPARE_GEQ.getOperatorPair(),
    T_COMPARE_LET.getOperatorPair(),
    T_COMPARE_GET.getOperatorPair(),
    T_COLON.getOperatorPair()
  };

}

#endif

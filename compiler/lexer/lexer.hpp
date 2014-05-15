#include <string>
#include <exception>
#include <boost/regex.hpp>
#include "./exceptions.hpp"
#include "./scanner.hpp"
#include "./fsm.hpp"

#ifndef LEXER_HPP

#define LEXER_HPP

namespace lexer {

  class Token {
  private:
    std::string name;
  public:
    Token() : name("nothing") {}
    Token(std::string _name) : name(_name) {}
    virtual std::string getDescription() { return "token: " + name; }
  };

  class Operator :  public Token {
    typedef std::pair<std::string, Operator*> OperatorPair;
  public:
    const std::string symbols;
    Operator(std::string _symbols) : symbols(_symbols) {};
    std::string getDescription() { return symbols; }
    OperatorPair getOperatorPair() { return OperatorPair(symbols, this); }
  };

  typedef FSMNode<Operator> OperatorFSM;
  typedef std::pair<std::string, Operator*> OperatorPair;
  typedef std::vector<OperatorPair> OperatorPairVector;

  class Keyword : public Token {
  public:
    const std::string symbol;
    Keyword(std::string _symbol) : symbol(_symbol) {}
    std::string getDescription() { return "keyword: " + symbol; }
  };

  class Identifier : public Token {
  public:
    const std::string name;
    Identifier(std::string _name) : name(_name) {}
    std::string getDescription() { return "identifier: " + name; }
  };

  typedef std::vector<Token*> TokenVector;
  typedef std::vector<Keyword*> KeywordVector;

  TokenVector tokenize(std::string input);

  class Tokenizer {
  private:
    TokenVector* tokens;
    OperatorFSM* operator_root;
    OperatorFSM* current_node;
    std::string current_token;
    void initialize();
    void flushKeyword();
    void flushOperator();
    void traverseOperatorFSM();
    void matchKeyword();
  public:
    Tokenizer();
    TokenVector tokenize(std::string input);
  };

  // utility methods
  bool isAlpha(char);
  bool isNumeric(char);
  bool isAlphaNumeric(char);

  extern OperatorFSM operatorFSM;
  extern KeywordVector keywordList;

  extern Token* T_INDENT;
  extern Token* T_UNINDENT;

  extern Operator* T_ASSIGN;
  extern Operator* T_COMPARE_EQUAL;

}

#endif

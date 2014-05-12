#include <string>
#include <exception>
#include <boost/regex.hpp>
#include "./fsm.hpp"

#ifndef LEXER_HPP

#define LEXER_HPP

namespace lexer {

  // extend exceptions from here if they have to do with
  // the lexer specifically
  class LexerException: public std::exception {
  public:
    const std::string message;
    LexerException(std::string _message) : message(_message) {}
  };

  class Token {
  public:
    virtual std::string getDescription() { return "nothing"; }
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
    void traverseOperatorFSM(char c);
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

}

#endif

#include <exception>
#include "exceptions.hpp"
#include "tokens.hpp"
#include "scanner.hpp"
#include "fsm.hpp"

#ifndef LEXER_HPP

#define LEXER_HPP

namespace lexer {

  class Tokenizer {
  private:
    static const Token& matchKeyword(StringScanner& scanner);
    static const Token& matchOperator(StringScanner& scanner);
  public:
    Tokenizer() {};
    TokenVector tokenize(std::string input);
  };

  // utility methods
  bool isAlpha(char);
  bool isNumeric(char);
  bool isAlphaNumeric(char);

  typedef FSMNode<const Operator> OperatorFSM;

  extern OperatorFSM operatorFSM;
}

#endif

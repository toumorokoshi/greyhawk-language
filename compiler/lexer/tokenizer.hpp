#include <exception>
#include "exceptions.hpp"
#include "tokens.hpp"
#include "scanner.hpp"
#include "fsm.hpp"
#include "utils.hpp"

#ifndef LEXER_TOKENIZER_HPP
#define LEXER_TOKENIZER_HPP

namespace lexer {

  typedef FSMNode<const Token> OperatorFSM;

  class Tokenizer {
  private:
    int indentation;
    void initialize();
    static const Token& matchKeyword(StringScanner& scanner);
    const Token& matchOperator(StringScanner& scanner);
    static const Token& matchNumber(StringScanner& scanner);
    void calculateIndent(StringScanner& scanner, TokenVector& tokens);
    void clearIndent(TokenVector& tokens);
  public:
    Tokenizer() {};
    TokenVector tokenize(std::istream& input);
  };

  OperatorFSM& getOperatorFSMRoot();
}

#endif

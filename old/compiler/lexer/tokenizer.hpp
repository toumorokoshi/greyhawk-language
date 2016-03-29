#include <exception>
#include "exceptions.hpp"
#include "tokens.hpp"
#include "scanner.hpp"
#include "fsm.hpp"
#include "utils.hpp"

#ifndef LEXER_TOKENIZER_HPP
#define LEXER_TOKENIZER_HPP

namespace lexer {

  class Tokenizer {
  private:
    int indentation;
    void initialize();
    static const Token& matchKeyword(StringScanner& scanner, int line);
    const Token& matchOperator(StringScanner& scanner, int line);
    static const Token& matchNumber(StringScanner& scanner, int line);
    char parseChar(StringScanner&, int line);
    void calculateIndent(StringScanner& scanner, TokenVector& tokens, int line);
    void clearIndent(TokenVector& tokens, int line);
  public:
    Tokenizer() {};
    TokenVector tokenize(std::istream& input);
  };

  FSMNode& getOperatorFSMRoot();
}

#endif

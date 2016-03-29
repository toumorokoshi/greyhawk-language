#include <exception>
#include <string>
#include "../exceptions.hpp"

#ifndef LEXER_EXCEPTIONS_HPP
#define LEXER_EXCEPTIONS_HPP

namespace lexer {

  // extend exceptions from here if they have to do with
  // the lexer specifically
  class LexerException: public core::GreyhawkException {
  public:
    LexerException(int _linum, std::string _message, std::string _specMessage) :
      GreyhawkException("line " + std::to_string(_linum) + ": " + _message,
                        _specMessage) {}
    virtual ~LexerException() throw() {}
  };

}

#endif

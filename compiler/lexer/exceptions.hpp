#include <exception>
#include <string>

#ifndef LEXER_EXCEPTIONS_HPP
#define LEXER_EXCEPTIONS_HPP

namespace lexer {

  // extend exceptions from here if they have to do with
  // the lexer specifically
  class LexerException: public std::exception {
  public:
    const std::string message;
    LexerException(std::string _message) : message(_message) {}
  };

}

#endif

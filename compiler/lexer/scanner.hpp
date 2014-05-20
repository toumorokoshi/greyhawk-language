#include <string>
#include <iostream>
#include "./exceptions.hpp"

#ifndef SCANNER_HPP
#define SCANNER_HPP

namespace lexer {

  class StringScannerException : public LexerException {
  public:
    StringScannerException(std::string _message): LexerException(_message) {}
  };

  class StringScanner {
  private:
    std::string source;
    std::string::iterator current;
    std::string::iterator end;

    void validateNextTokenExists() {
      if (current == end) {
        throw StringScannerException("No next token exists!");
      }
    }

  public:

    StringScanner(std::string _source) :
      source(_source),
      current(source.begin()),
      end(source.end()) {}

    char peek() {
      validateNextTokenExists();
      return *current;
    }

    char next() {
      validateNextTokenExists();
      return *(current++);
    }

    bool hasNext() {
      return (current != end);
    }

  };
}

#endif

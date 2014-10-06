#include <string>
#include <iostream>
#include "exceptions.hpp"

#ifndef SCANNER_HPP
#define SCANNER_HPP

namespace lexer {

  class StringScannerException : public core::GreyhawkException {
  public:
    StringScannerException(std::string _message): core::GreyhawkException(_message) {}
    virtual ~StringScannerException() throw () {}
  };

  class StringScanner {
  private:
    std::istream& source;

    void validateNextTokenExists() {
      if (source.peek() == EOF) {
        throw StringScannerException("No next token exists!");
      }
    }

  public:

    StringScanner(std::istream& _source) :
      source(_source) {}

    char peek() {
      validateNextTokenExists();
      return source.peek();
    }

    char next() {
      validateNextTokenExists();
      return source.get();
    }

    void back() {
      source.unget();
    }

    bool hasNext() {
      return (source.peek() != EOF);
    }

  };
}

#endif

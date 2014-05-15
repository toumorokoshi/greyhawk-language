#include <string>
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
    std::string::iterator begin;
    std::string::iterator current;
    std::string::iterator end;

    void validateNextTokenExists() {
      if (current == end) {
        throw StringScannerException("No next token exists!");
      }
    }

  public:

    StringScanner(std::string _source) :
      begin(_source.begin()),
      current(_source.begin()),
      end(_source.end()) {}

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

#include "../exceptions.hpp"
#include "../lexer/tokens.hpp"
#ifndef PARSER_EXCEPTIONS_HPP
#define PARSER_EXCEPTIONS_HPP

namespace parser {

  // extend exceptions from here if they have to do with
  // the Parser specifically
  class ParserException: public greyhawk::GreyhawkException {
  public:
    ParserException(std::string _message)
      : GreyhawkException(_message) {}

    ParserException(const lexer::Token& t, std::string _message)
      : GreyhawkException("line " + std::to_string(t.line) +
                          " on " + t.getDescription() +
                          ": (parser) " + _message) {}

    virtual ~ParserException() throw () {}
  };

}

#endif

#include "../exceptions.hpp"
#ifndef PARSER_EXCEPTIONS_HPP
#define PARSER_EXCEPTIONS_HPP

namespace parser {

  // extend exceptions from here if they have to do with
  // the Parser specifically
  class ParserException: public greyhawk::GreyhawkException {
  public:
    ParserException(std::string _message)
      : GreyhawkException("parser: " + _message) {};
  };

}

#endif

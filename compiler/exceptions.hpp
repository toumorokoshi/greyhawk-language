#include <exception>
#include <string>

#ifndef GREYHAWK_EXCEPTIONS_HPP
#define GREYHAWK_EXCEPTIONS_HPP

namespace core {

  // any exception from greyhawk extends here
  class GreyhawkException: public std::exception {
  public:
    const std::string message;
    const std::string specMessage;
    GreyhawkException(std::string _message) : message(_message) {}
    GreyhawkException(std::string _message, std::string _specMessage) :
      message(_message),
      specMessage(_specMessage) {}
    virtual ~GreyhawkException() throw () {}
  };
}

#endif

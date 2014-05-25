#include <exception>
#include <string>

#ifndef GREYHAWK_EXCEPTIONS_HPP
#define GREYHAWK_EXCEPTIONS_HPP

namespace greyhawk {

  // any exception from greyhawk extends here
  class GreyhawkException: public std::exception {
  public:
    const std::string message;
    GreyhawkException(std::string _message) : message(_message) {}
  };

}

#endif

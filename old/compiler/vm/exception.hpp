#include <exception>
#include <string>

#ifndef CODEGEN_EXCEPTIONS_HPP
#define CODEGEN_EXCEPTIONS_HPP

namespace VM {

  // extend exceptions from here if they have to do with
  // the lexer specifically
  class CodeGenException: public std::exception {
  public:
    const std::string message;
    CodeGenException(std::string _message) : message(_message) {}
    virtual ~CodeGenException() throw() {}
  };

}

#endif

#include <exception>
#include <string>

#ifndef VM_EXCEPTIONS_HPP
#define VM_EXCEPTIONS_HPP

namespace VM {

  class VMException: public std::exception {
  public:
    const std::string message;
    VMException(std::string _message) :
      message(_message) {}
    virtual const char* what() const noexcept { return message.c_str(); }
  };

}

#endif

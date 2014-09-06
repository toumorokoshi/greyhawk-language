#include "scope.hpp"

#ifndef VM_STATEMENT_HPP
#define VM_STATEMENT_HPP

namespace VM {

  class VMStatement {
  public:
    virtual void execute(VMScope& scope) = 0;
    virtual ~VMStatement() {}
  };
}

#endif

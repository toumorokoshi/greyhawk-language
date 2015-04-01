#include "scope.hpp"
#include "exceptions.hpp"

#ifndef VM_STATEMENT_HPP
#define VM_STATEMENT_HPP

namespace VM {

  class VMStatement {
  public:
    virtual VMObject* execute(VMScope& scope) = 0;
    virtual ~VMStatement() {}
  };
}

#endif

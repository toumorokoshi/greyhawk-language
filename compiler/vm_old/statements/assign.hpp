#include "../statement.hpp"
#include "../expression.hpp"
#include "../exceptions.hpp"

#ifndef VM_ASSIGN_HPP
#define VM_ASSIGN_HPP

namespace VM {

  class VMAssign : public VMStatement {
  public:
    const std::string name;
    VMExpression* expression;

    VMAssign(std::string _name, VMExpression* _expression) :
      name(_name), expression(_expression) {}

    virtual VMObject* execute(VMScope& scope) {
      scope.locals[name] = expression->evaluate(scope);
      return NULL;
    }
  };
}

#endif

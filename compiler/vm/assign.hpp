#include "statement.hpp"
#include "expression.hpp"
#include "exceptions.hpp"

#ifndef VM_ASSIGN_HPP
#define VM_ASSIGN_HPP

namespace VM {

  class VMAssign : public VMStatement {
  public:
    const std::string name;
    VMExpression* expression;

    VMAssign(std::string _name, VMExpression* _expression) :
      name(_name), expression(_expression) {}

    virtual void execute(VMScope& scope) {
      if (scope.locals.find(name) == scope.locals.end()) {
        throw VMException("'" + name + "' is not declared!");
      }

      VMObject* new_value = expression->evaluate(scope);
      VMObject* variable = scope.locals[name];
      // TODO: don't evaluate to get return type
      if (!variable->getType()->matches(new_value->getType())) {
        throw VMException("cannot assign variable to a different type!");
      }

      scope.locals[name] = new_value;
    }
  };
}

#endif

#include "statement.hpp"
#include "expression.hpp"
#include "exceptions.hpp"

#ifndef VM_DECLARE_HPP
#define VM_DECLARE_HPP

namespace VM {

  class VMDeclare : public VMStatement {
  public:
    const std::string name;
    VMExpression* expression;

    VMDeclare(std::string _name, VMExpression* _expression) :
      name(_name), expression(_expression) {}

    virtual VMObject* execute(VMScope& scope) {
      if (scope.locals.find(name) != scope.locals.end()) {
        throw VMException("cannot declare variable '" + name + "' twice!");
      }

      scope.locals[name] = expression->evaluate(scope);
      return NULL;
    }
  };
}

#endif

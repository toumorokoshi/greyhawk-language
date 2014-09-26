#include "../statement.hpp"
#include "../expression.hpp"


#ifndef VM_STATEMENT_RETURN_HPP
#define VM_STATEMENT_RETURN_HPP

namespace VM {

  class VMReturn : public VMStatement {
  public:
    VMExpression* returnExpression;
    virtual VMObject* execute(VMScope& scope) {
      return returnExpression->evaluate(scope);
    }

    VMReturn(VMExpression* _returnExpression) :
      returnExpression(_returnExpression) {}
  };
}

#endif

#include "block.hpp"
#include "statement.hpp"
#include "expression.hpp"

#ifndef VM_IFELSE_HPP
#define VM_IFELSE_HPP

namespace VM {

  class VMIfElse : public VMStatement {
  public:
    VMExpression * ifStatement;
    VMBlock* trueBlock;
    VMBlock* falseBlock;
    virtual VMObject* execute(VMScope& scope);
    VMIfElse(VMExpression* _ifStatement,
             VMBlock* _trueBlock,
             VMBlock* _falseBlock) :
      ifStatement(_ifStatement),
      trueBlock(_trueBlock),
      falseBlock(_falseBlock) {}
  };

}


#endif

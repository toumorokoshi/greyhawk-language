#include "block.hpp"
#include "statement.hpp"
#include "expression.hpp"

#ifndef VM_FOR_HPP
#define VM_FOR_HPP

namespace VM {

  class VMForLoop : public VMStatement {
  public:
    VMBlock* block;
    std::string variableName;
    VMExpression* iterableObjectExpression;
    virtual VMObject* execute(VMScope& scope);
    VMForLoop(VMBlock* _block,
              std::string _variableName,
              VMExpression* _iterableObjectExpression) :

      block(_block),
      variableName(_variableName),
      iterableObjectExpression(_iterableObjectExpression) {}
  };
}

#endif

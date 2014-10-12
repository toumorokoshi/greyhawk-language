#include "../expression.hpp"
#include "../basic_types/array.hpp"

#ifndef VM_HPP_EXPRESSIONS_ARRAYEXPRESSION
#define VM_HPP_EXPRESSIONS_ARRAYEXPRESSION

namespace VM {

  class VMArrayExpression: public VMExpression {
  public:
    virtual VMObject* evaluate(VMScope&);
    VMArrayExpression(VMClass* _arrayType,
                      std::vector<VMExpression*>& _elements) :
      arrayType(_arrayType), elements(_elements) {}
  private:
    VMClass* arrayType;
    std::vector<VMExpression*>& elements;
  };
}

#endif

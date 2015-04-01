#include "arrayexpression.hpp"

namespace VM {

  VMObject* VMArrayExpression::evaluate(VMScope& scope) {
    auto size = elements.size();
    auto evaledElements = new VMObject*[size];
    for (int i = 0; i < size; i++) {
      evaledElements[i] = elements[i]->evaluate(scope);
    }
    return new VMArray(arrayType, elements.size(), evaledElements);
  };
}

#include "for.hpp"
#include "basic_types/bool.hpp"

namespace VM {

  void VMForLoop::execute(VMScope& scope) {
    std::vector<VMObject*> noArgs;
    VMScope localScope(&scope);

    auto iterableObject = iterableObjectExpression->evaluate(scope);

    while(((VMBool*) iterableObject->call("hasNext", noArgs))->value) {
      localScope.locals[variableName] = iterableObject->call("next", noArgs);
      block->execute(localScope);
    }
  }

}

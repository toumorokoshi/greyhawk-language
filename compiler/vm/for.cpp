#include "for.hpp"
#include "basic_types/bool.hpp"

namespace VM {

  VMObject* VMForLoop::execute(VMScope& scope) {
    std::vector<VMObject*> noArgs;
    VMScope localScope(&scope);

    auto iterableObject = iterableObjectExpression->evaluate(scope);

    while(((VMBool*) iterableObject->call("hasNext", noArgs))->value) {
      localScope.locals[variableName] = iterableObject->call("next", noArgs);
      auto returnObject = block->execute(localScope);

      if (returnObject != NULL) { return returnObject; }
    }

    return NULL;
  }

}

#include "ifelse.hpp"
#include "utils.hpp"

namespace VM {

  VMObject* VMIfElse::execute(VMScope& scope) {
    if (evaluate_vmbool(ifStatement->evaluate(scope))) {
      return trueBlock->execute(scope);
    } else {
      return falseBlock->execute(scope);
    }
  }

}

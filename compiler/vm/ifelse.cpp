#include "ifelse.hpp"
#include "utils.hpp"

namespace VM {

  void VMIfElse::execute(VMScope& scope) {
    if (evaluate_vmbool(ifStatement->evaluate(scope))) {
      trueBlock->execute(scope);
    } else {
      falseBlock->execute(scope);
    }
  }

}

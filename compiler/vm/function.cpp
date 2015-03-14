#include "function.hpp"
#include "execution_engine.hpp"

namespace VM {

  GValue* GFunction::execute(GModules* modules, G2ScopeInstance& scope) {
    executeInstructions(modules, instructions, scope);
    return new GValue{.asNone = 0};
  }
}

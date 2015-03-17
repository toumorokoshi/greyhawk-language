#include "function.hpp"
#include "execution_engine.hpp"

namespace VM {

  GValue* GFunction::execute(GModules* modules, GScopeInstance& scope) {
    executeInstructions(modules, instructions, scope);
    return new GValue{.asNone = 0};
  }
}

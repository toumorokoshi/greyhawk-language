#include "function.hpp"
#include "execution_engine.hpp"

namespace VM {

  GValue GFunction::execute(GModules* modules, GEnvironmentInstance& scope) {
    executeInstructions(modules, instructions, scope);
    return *getNoneObject();
  }
}

#include "function.hpp"
#include "execution_engine.hpp"

namespace VM {

  GObject GFunction::execute(GModules* modules, GScopeInstance& scope) {
    executeInstructions(modules, instructions, scope);
    return *getNoneObject();
  }
}

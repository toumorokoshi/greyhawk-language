#include "rootenvironment.hpp"

namespace VM {

  GEnvironment& getBaseEnvironment() {
    auto static environment = new GEnvironment();
    auto static _initialized = false;
    if (!_initialized) {
      environment->addObject("__builtins__", getBuiltinModuleType());
      _initialized = true;
    }
    return *environment;
  }

  GEnvironmentInstance& getBaseEnvironmentInstance() {
    auto static envInst =                                             \
      getBaseEnvironment().createInstance(*new GEnvironmentInstance{});
    auto static _initialized = false;
    if (!_initialized) {
      envInst->locals[0].asModule = getBuiltins();
      _initialized = true;
    }
    return *envInst;
  }
}

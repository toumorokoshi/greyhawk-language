#include "function.hpp"
#include "execution_engine.hpp"

namespace VM {

  GFunctionInstance* GFunction::createInstance(GEnvironmentInstance& parentEnvironment) {
    return new GFunctionInstance {
      .function = this,
      .parentEnv = parentEnvironment
    };
  }
}

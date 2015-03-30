#include "function.hpp"
#include "execution_engine.hpp"

namespace VM {

  GValue GFunctionInstance::execute(GModules* modules) {
    auto environmentInstance = function->environment->createInstance(parentEnv);
    return executeInstructions(modules,function->instructions,
                               *environmentInstance);
  }

  GFunctionInstance* GFunction::createInstance(GEnvironmentInstance& parentEnvironment) {
    return new GFunctionInstance {
      .function = this,
      .parentEnv = parentEnvironment
    };
  }
}

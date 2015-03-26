#include "function.hpp"
#include "execution_engine.hpp"

namespace VM {

  GValue GFunctionInstance::execute(GModules* modules) {
    return executeInstructions(modules,function->instructions,
                               *environmentInstance);
  }

  GFunctionInstance* GFunction::createInstance(GEnvironmentInstance& parentEnvironment) {
    auto environmentInstance = environment.createInstance(parentEnvironment);
    return new GFunctionInstance {
      .function = this,
      .environmentInstance = environmentInstance
    };
  }
}

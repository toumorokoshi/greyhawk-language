#include <vector>
#include "../exceptions.hpp"
#include "environment.hpp"

namespace VM {

  int GEnvironment::allocateClass(GType* type) {
    types.push_back(type);
    return typeCount++;
  }

  int GEnvironment::allocateFunction(GFunction* func) {
    functions.push_back(func);
    return functionCount++;
  }

  GIndex* GEnvironment::allocateObject(GType* type) {
    localsTypes.push_back(type);
    return new GIndex {
      .registerNum = localsCount++,
      .type = type
    };
  }

  GIndex* GEnvironment::getGlobal(std::string name) {
    if (globalsByName.find(name) != globalsByName.end()) {
      int index = globalsByName[name];
      return new GIndex {
        .indexType = GLOBAL,
        .registerNum = index,
        .type = globalsTypes[index]
      };
    }

    return NULL;
  }

  GEnvironmentInstance* GEnvironment::createInstance(GEnvironmentInstance& parent) {
    auto globals = new GValue*[globalsCount];

    for (int i = 0; i < globalsCount; i++) {
      auto index = indicesInParent[i];
      bool globalValue = index < 0;
      if (globalValue) {
        globals[i] = parent.globals[-(index + 1)];
      } else {
        globals[i] = &(parent.locals[index]);
      }
    }

    return new GEnvironmentInstance {
      .environment = this,
      .globals = globals,
      .locals = new GValue[localsCount]
    };
  }

  GEnvironment* getEmptyEnvironment() {
    auto static environment = new GEnvironment();
    return environment;
  }

  GEnvironmentInstance& getEmptyEnvironmentInstance() {
    auto static environment = new GEnvironmentInstance();
    return *environment;
  }
}

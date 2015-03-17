#include "scope.hpp"

namespace VM {

  GScopeInstance GScope::createInstance() {
    return GScopeInstance {
      .scope = this, .values = new GValue[localsCount]
    };
  }

  GValue GScopeInstance::getValue(std::string name) {

    if (scope->localsTable.find(name) != scope->localsTable.end()) {
      return values[scope->localsTable[name]];
    }

    if (scope->globalsTable.find(name) != scope->globalsTable.end()) {
      return *(scope->globals[scope->globalsTable[name]]);
    }

    return GValue {0};
  }
}

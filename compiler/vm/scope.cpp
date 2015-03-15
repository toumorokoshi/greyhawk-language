#include "scope.hpp"

namespace VM {

  GOldFunction* GScope::getFunction(std::string name) {
    if (functionTable.find(name) != functionTable.end()) {
      return functionTable[name];
    } else if (_parent != NULL) {
      return _parent->getFunction(name);
    }
    return NULL;
  }

  void G2Scope::addLocal(std::string name) {
    localsTable[name] = localsCount++;
  }

  G2ScopeInstance G2Scope::createInstance() {
    return G2ScopeInstance {
      .scope = this, .values = new GValue[localsCount]
    };
  }

  GValue G2ScopeInstance::getValue(std::string name) {

    if (scope->localsTable.find(name) != scope->localsTable.end()) {
      return values[scope->localsTable[name]];
    }

    if (scope->globalsTable.find(name) != scope->globalsTable.end()) {
      return *(scope->globals[scope->globalsTable[name]]);
    }

    return GValue {0};
  }
}

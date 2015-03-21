#include <vector>
#include "../exceptions.hpp"
#include "scope.hpp"

namespace VM {

  GScopeInstance GScope::createInstance() {
    return GScopeInstance {
      .scope = this, .values = new GObject[localsCount]
    };
  }

  GObject GScopeInstance::getValue(std::string name) {

    if (scope->localsTable.find(name) != scope->localsTable.end()) {
      return values[scope->localsTable[name]];
    }

    if (scope->globalsTable.find(name) != scope->globalsTable.end()) {
      return *(scope->globals[scope->globalsTable[name]]);
    }

    throw 1;
  }

  GScope* GScopeInstance::createChildScope() {
    auto childScope = new GScope();

    auto globals = new std::vector<GObject*>();
    int globalsCount = 0;

    for (auto& kv: scope->localsTable) {
      childScope->globalsTable[kv.first] = globalsCount++;
      globals->push_back(&(values[kv.second]));
    }

    for (auto& kv : scope->globalsTable) {
      if (childScope->globalsTable.find(kv.first) != childScope->globalsTable.end()) {
        childScope->globalsTable[kv.first] = globalsCount++;
        globals->push_back(scope->globals[kv.second]);
      }
    }

    childScope->globals = &(*globals)[0];
    childScope->globalsCount = globalsCount;
    return childScope;
  }
}

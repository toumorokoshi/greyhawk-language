#include <vector>
#include "../exceptions.hpp"
#include "scope.hpp"

namespace VM {

  GScope GScope::createChild() {
    std::map<std::string, int> childGlobalsTable;
    auto childGlobalsTypes = new std::vector<GType*>();
    auto childIndicesInParent = new std::vector<int>();
    int childGlobalsCount = 0;

    // locals become globals
    for (auto &kv: localsTable) {
      childGlobalsTypes->push_back(globalsTypes[kv.second]);
      childIndicesInParent->push_back(-kv.second);
      childGlobalsTable[kv.first] = childGlobalsCount++;
    }

    for (auto &kv: globalsTable) {
      if (childGlobalsTable.find(kv.first) == childGlobalsTable.end()) {
        childGlobalsTypes->push_back(localsTypes[kv.second]);
        childIndicesInParent->push_back(kv.second);
        childGlobalsTable[kv.first] = childGlobalsCount++;
      }
    }

    return GScope {
      .globalsTable = childGlobalsTable,
      .globalsTypes = &((*childGlobalsTypes)[0]),
      .indicesInParent = &((*childIndicesInParent)[0]),
      .globalsCount = childGlobalsCount
    };
  }

  GScopeInstance GScope::createInstance(GScopeInstance& parent) {
    auto globals = new GValue*[globalsCount];

    for (int i = 0; i < globalsCount; i++) {
      auto index = indicesInParent[i];
      bool globalValue = index < 0;
      if (globalValue) {
        globals[i] = parent.globals[-index];
      } else {
        globals[i] = &(parent.locals[index]);
      }
    }

    return GScopeInstance {
      .scope = this,
      .globals = globals,
      .locals = new GValue[localsCount]
    };
  }

  GValue GScopeInstance::getValue(std::string name) {

    if (scope->localsTable.find(name) != scope->localsTable.end()) {
      return locals[scope->localsTable[name]];
    }

    if (scope->globalsTable.find(name) != scope->globalsTable.end()) {
      return *(globals[scope->globalsTable[name]]);
    }

    throw 1;
  }
}

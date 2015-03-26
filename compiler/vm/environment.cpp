#include <vector>
#include "../exceptions.hpp"
#include "environment.hpp"

namespace VM {

  GEnvironment GEnvironment::createChild() {
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

    return GEnvironment {
      .globalsTable = childGlobalsTable,
      .globalsTypes = &((*childGlobalsTypes)[0]),
      .indicesInParent = &((*childIndicesInParent)[0]),
      .globalsCount = childGlobalsCount
    };
  }

  GEnvironmentInstance GEnvironment::createInstance(GEnvironmentInstance& parent) {
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

    return GEnvironmentInstance {
      .scope = this,
      .globals = globals,
      .locals = new GValue[localsCount]
    };
  }

  GValue GEnvironmentInstance::getValue(std::string name) {

    if (scope->localsTable.find(name) != scope->localsTable.end()) {
      return locals[scope->localsTable[name]];
    }

    if (scope->globalsTable.find(name) != scope->globalsTable.end()) {
      return *(globals[scope->globalsTable[name]]);
    }

    throw 1;
  }
}

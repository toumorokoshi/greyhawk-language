#include <vector>
#include "../exceptions.hpp"
#include "environment.hpp"

namespace VM {

  GEnvironment* GEnvironment::createChild() {
    std::map<std::string, int> childGlobalsTable;
    auto childGlobalsTypes = new std::vector<GType*>();
    auto childIndicesInParent = new std::vector<int>();
    int childGlobalsCount = 0;

    // locals become globals
    for (auto &kv: localsTable) {
      childGlobalsTypes->push_back(localsTypes[kv.second]);
      childIndicesInParent->push_back(kv.second);
      childGlobalsTable[kv.first] = childGlobalsCount++;
    }

    for (auto &kv: globalsTable) {
      if (childGlobalsTable.find(kv.first) == childGlobalsTable.end()) {
        childGlobalsTypes->push_back(globalsTypes[kv.second]);
        childIndicesInParent->push_back(-(kv.second + 1));
        childGlobalsTable[kv.first] = childGlobalsCount++;
      }
    }

    auto environment = new GEnvironment {
      .globalsTable = childGlobalsTable,
      .globalsCount = childGlobalsCount
    };

    environment->functionByName.insert(functionByName.begin(), functionByName.end());
    environment->typeByName.insert(typeByName.begin(), typeByName.end());

    if (childGlobalsCount > 0) {
      environment->globalsTypes = &((*childGlobalsTypes)[0]);
      environment->indicesInParent = &((*childIndicesInParent)[0]);
    }
    return environment;
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

  GValue GEnvironmentInstance::getValue(std::string name) {

    if (environment->localsTable.find(name) != environment->localsTable.end()) {
      return locals[environment->localsTable[name]];
    }

    if (environment->globalsTable.find(name) != environment->globalsTable.end()) {
      return *(globals[environment->globalsTable[name]]);
    }

    throw 1;
  }

  GEnvironment* getEmptyEnvironment() {
    auto static environment = new GEnvironment();
    auto static _initialized = false;
    if (!_initialized) {
      environment->globalsTypes = new GType*[0];
    }
  }

  GEnvironmentInstance& getEmptyEnvironmentInstance() {
    auto static environment = new GEnvironmentInstance();
    return *environment;
  }
}

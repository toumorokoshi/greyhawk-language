#include <vector>
#include "../exceptions.hpp"
#include "environment.hpp"

#ifdef DEBUG
  #define debug(s) std::cerr << s << std::endl;
#else
  #define debug(s);
#endif

namespace VM {

  // object methods
  GIndex* GEnvironment::addObject(std::string name, GType* type) {
    auto index = allocateObject(type);
    localsByName[name] = index->registerNum;
    return index;
  }

  GIndex* GEnvironment::allocateObject(GType* type) {
    localsTypes.push_back(type);
    return new GIndex {
      .registerNum = localsCount++,
      .type = type
    };
  }

  GIndex* GEnvironment::getObject(std::string name) {
    if (localsByName.find(name) != localsByName.end()) {
      int index = localsByName[name];
      return new GIndex {
        .registerNum = index,
        .type = localsTypes[index]
      };
    }

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

  // class methods
  int GEnvironment::allocateClass(GType* cls) {
    classes.push_back(cls);
    return classesCount++;
  }

  GIndex* GEnvironment::addClass(std::string name, VM::GType* cls) {
    auto classIndex = allocateClass(cls);
    classesByName[name] = classIndex;
    return new GIndex {
      .registerNum = classIndex,
      .type = getClassType()
    };
  }

  GType* GEnvironment::getClass(std::string name) {
    if (classesByName.find(name) != classesByName.end()) {
      return classes[classesByName[name]];
    }
    return NULL;
  }

  // function methods
  GIndex* GEnvironment::addFunction(std::string name, GFunction* func) {
    int functionIndex = allocateFunction(func);
    debug(functionIndex);
    functionsByName[name] = functionIndex;
    auto index = addObject(name, getFunctionType());
    return index;
  }

  int GEnvironment::allocateFunction(GFunction* func) {
    functions.push_back(func);
    return functionsCount++;
  }

  GFunction* GEnvironment::getFunction(std::string name) {
    if (functionsByName.find(name) != functionsByName.end()) {
      return functions[functionsByName[name]];
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

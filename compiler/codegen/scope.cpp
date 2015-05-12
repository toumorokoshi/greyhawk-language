#include "scope.hpp"
#include "../parser/nodes.hpp"

#ifdef DEBUG
  #define debug(s) std::cerr << s << std::endl;
#else
  #define debug(s);
#endif

using namespace VM;

namespace codegen {

  // a lot of methods are repeated from GEnvironment.
  // they should all probably be in addObject, but
  // I want to wait to see if they actually belong here.
  GIndex* GScope::addObject(std::string name, VM::GType* type) {
    if (parentScope == NULL) {
      return environment->addObject(name, type);
    } else {
      auto val = environment->allocateObject(type);
      localsByName[name] = val;
      return val;
    }
  }

  GIndex* GScope::allocateObject(VM::GType* type) {
    return environment->allocateObject(type);
  }

  GIndex* GScope::getObject(std::string name) {
    if (localsByName.find(name) != localsByName.end()) {
      return localsByName[name];
    } else if (parentScope != NULL) {
      return parentScope->getObject(name);
    } else {
      return environment->getObject(name);
    }
  }

  GIndex* GScope::addClass(std::string name, VM::GType* type) {
    if (parentScope == NULL) {
      return environment->addClass(name, type);
    } else {
      auto typeIndex = environment->allocateClass(type);
      typeIndexByName[name] = typeIndex;
      return new GIndex {
        .registerNum = typeIndex,
        .type = getClassType()
      };
    }
  }

  GType* GScope::getClass(std::string name) {
    GType* type = NULL;

    if (typeIndexByName.find(name) != typeIndexByName.end()) {
      type = environment->classes[typeIndexByName[name]];
    } else if (parentScope != NULL) {
      type = parentScope->getClass(name);
    } else {
      type = environment->getClass(name);
    }

    return type;
  }

  GIndex* GScope::addFunction(std::string name, GFunction* func,
                              parser::PFunctionDeclaration* declaration) {
    if (parentScope == NULL) {
      functionDeclarations.push_back(declaration);
      return environment->addFunction(name, func);

    } else {
      int functionIndex = environment->allocateFunction(func);
      functionIndexByName[name] = functionIndex;
      functionDeclarations.push_back(declaration);
      auto index = addObject(name, VM::getFunctionType());
      return index;
    }
  }


  // function methods
  GFunction* GScope::getFunction(std::string name) {
    GFunction* function = NULL;
    if (functionIndexByName.find(name) != functionIndexByName.end()) {
      function = environment->functions[functionIndexByName[name]];
    } else if (parentScope != NULL) {
      function = parentScope->getFunction(name);
    } else {
      function = environment->getFunction(name);
    }
    return function;
  }

  GScope* GScope::createChild(bool isRootScope) {
    GEnvironment* childEnvironment;
    GScope* parentScope = NULL;

    if (isRootScope) {
      childEnvironment = createEnvironmentFromScope(this);
    } else {
      childEnvironment = environment;
      parentScope = this;
    }

    return new GScope {
      .environment = childEnvironment,
      .parentScope = parentScope
    };
  }

  void GScope::finalize() {
    for (auto& funcDecl : functionDeclarations) {
      auto funcIndex = functionIndexByName[funcDecl->name];
      auto function = environment->functions[funcIndex];
      funcDecl->generateBody(function, this);
    }
  }

  GEnvironment* createEnvironmentFromScope(GScope* scope) {
    debug("YYY: create child was called");
    std::map<std::string, int> globalsByName;
    auto globalsTypes = new std::vector<GType*>();
    auto indicesInParent = new std::vector<int>();
    int globalsCount = 0;

    auto parentEnvironment = scope->environment;

    // locals become globals
    auto localsScope = scope;
    while (localsScope != NULL) {
      auto locals = localsScope->localsByName;
      for (auto &kv : locals) {
        if (globalsByName.find(kv.first) == globalsByName.end()) {
          auto valIndex = kv.second->registerNum;
          auto valType = parentEnvironment->localsTypes[valIndex];
          indicesInParent->push_back(valIndex);
          globalsTypes->push_back(valType);
          globalsByName[kv.first] = globalsCount++;
        }
      }

      localsScope = scope->parentScope;
    }

    for (auto &kv: parentEnvironment->localsByName) {
      debug("Y: checking " << kv.first);
      if (globalsByName.find(kv.first) == globalsByName.end()) {
        debug("Y: adding " << kv.first);
        auto valIndex = kv.second;
        auto valType = parentEnvironment->localsTypes[valIndex];
        indicesInParent->push_back(kv.second);
        globalsTypes->push_back(valType);
        globalsByName[kv.first] = globalsCount++;
      }
    }

    for (auto &kv: parentEnvironment->globalsByName) {
      debug("Y: checking " << kv.first);
      if (globalsByName.find(kv.first) == globalsByName.end()) {
        debug("Y: adding " << kv.first);
        auto valIndex = kv.second;
        auto valType = parentEnvironment->globalsTypes[valIndex];
        indicesInParent->push_back(-(kv.second + 1));
        globalsTypes->push_back(valType);
        globalsByName[kv.first] = globalsCount++;
      }
    }

    auto environment = new GEnvironment();
    environment->globalsByName = globalsByName;
    environment->globalsCount = globalsCount;
    /* auto environment = new GEnvironment {
      .globalsByName = globalsByName,
      .globalsCount = globalsCount,
      }; */

    if (globalsCount > 0) {
      environment->indicesInParent = &((*indicesInParent)[0]);
      environment->globalsTypes = &((*globalsTypes)[0]);
    }

    environment->functions.insert(environment->functions.end(),
                                  parentEnvironment->functions.begin(),
                                  parentEnvironment->functions.end());

    environment->classes.insert(environment->classes.end(),
                                parentEnvironment->classes.begin(),
                                parentEnvironment->classes.end());

    // return environment;
    return environment;
  }
}

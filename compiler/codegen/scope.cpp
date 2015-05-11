#include "scope.hpp"
#include "../parser/nodes.hpp"

using namespace VM;

namespace codegen {

  GIndex* GScope::addClass(std::string name, VM::GType* type) {
    auto typeIndex = environment->allocateClass(type);
    typeIndexByName[name] = typeIndex;
    return new GIndex {
      .registerNum = typeIndex,
      .type = getClassType()
    };
  }

  GIndex* GScope::addFunction(std::string name, GFunction* func,
                              parser::PFunctionDeclaration* declaration) {
    int functionIndex = environment->allocateFunction(func);
    functionDeclarations.push_back(declaration);

    auto index = addObject(name, VM::getFunctionType());
    functionIndexByName[name] = functionIndex;
    return index;
  }

  // a lot of methods are repeated from GEnvironment.
  // they should all probably be in addObject, but
  // I want to wait to see if they actually belong here.
  VM::GIndex* GScope::addObject(std::string name, VM::GType* type) {
    auto val = environment->allocateObject(type);
    localsByName[name] = val;
    return val;
  }

  VM::GIndex* GScope::allocateObject(VM::GType* type) {
    return environment->allocateObject(type);
  }

  GType* GScope::getClass(std::string name) {
    GType* type = NULL;
    if (typeIndexByName.find(name) != typeIndexByName.end()) {
      type = environment->types[typeIndexByName[name]];
    } else if (parentScope != NULL) {
      type = parentScope->getClass(name);
    }
    return type;
  }

  // function methods
  GFunction* GScope::getFunction(std::string name) {
    GFunction* function = NULL;
    if (functionIndexByName.find(name) != functionIndexByName.end()) {
      function = environment->functions[functionIndexByName[name]];
    } else if (parentScope != NULL) {
      function = parentScope->getFunction(name);
    }
    return function;
  }

  GIndex* GScope::getObject(std::string name) {
    if (localsByName.find(name) != localsByName.end()) {
      return localsByName[name];
    } else if (parentScope != NULL) {
      return parentScope->getObject(name);
    } else {
      return environment->getGlobal(name);
    }
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


    for (auto &kv: parentEnvironment->globalsByName) {
      if (globalsByName.find(kv.first) == globalsByName.end()) {
        auto valIndex = kv.second;
        auto valType = parentEnvironment->globalsTypes[valIndex];
        indicesInParent->push_back(-(kv.second + 1));
        globalsTypes->push_back(valType);
        globalsByName[kv.first] = globalsCount++;
      }
    }

    auto environment = new GEnvironment {
      .globalsByName = globalsByName,
      .globalsCount = globalsCount,
    };

    if (globalsCount > 0) {
      environment->indicesInParent = &((*indicesInParent)[0]);
      environment->globalsTypes = &((*globalsTypes)[0]);
    }

    environment->functions.insert(environment->functions.end(),
                                  parentEnvironment->functions.begin(),
                                  parentEnvironment->functions.end());

    environment->types.insert(environment->types.end(),
                              parentEnvironment->types.begin(),
                              parentEnvironment->types.end());

    return environment;
  }
}

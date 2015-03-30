#include <map>
#include "../vm/environment.hpp"

#ifndef CODEGEN_SCOPE_HPP
#define CODEGEN_SCOPE_HPP

namespace parser {
  class PFunctionDeclaration;
}

namespace codegen {

  class GScope {
  public:
    VM::GEnvironment* environment;
    std::map<std::string, VM::GIndex*> localMap;
    std::vector<VM::GFunction*> functions;
    std::vector<parser::PFunctionDeclaration*> functionDeclarations;
    bool isInnerScope;

    // a lot of methods are repeated from GEnvironment.
    // they should all probably be in addObject, but
    // I want to wait to see if they actually belong here.
    VM::GIndex* addObject(std::string name, VM::GType* type) {
      if (isInnerScope) {
        auto val = environment->allocateObject(type);
        localMap[name] = val;
        return val;
      } else {
        return environment->addObject(name, type);
      }
    }

    VM::GIndex* allocateObject(VM::GType* type) {
      return environment->allocateObject(type);
    }

    VM::GIndex* getObject(std::string name) {
      if (localMap.find(name) != localMap.end()) {
        return localMap[name];
      }
      return environment->getObject(name);
    }

    int allocateFunction(VM::GFunction* function) {
      return environment->allocateFunction(function);
    }

    VM::GFunction* getFunction(std::string name) {
      if (localMap.find(name) != localMap.end()) {
        auto functionInstanceIndex = localMap[name];
        if (environment->functionTable.find(functionInstanceIndex->registerNum) !=
            environment->functionTable.end()) {
          return environment->functions[environment->functionTable[functionInstanceIndex->registerNum]];
        }
      }

      return environment->getFunction(name);
    }

    VM::GIndex* addFunction(std::string name, VM::GFunction* func,
                            parser::PFunctionDeclaration* declaration) {
      functions.push_back(func);
      functionDeclarations.push_back(declaration);
      int functionIndex;
      if (isInnerScope) {
        functionIndex = environment->allocateFunction(func);
      } else {
        functionIndex = environment->addFunction(name, func);
      }
      auto index = addObject(name, VM::getFunctionType());
      environment->functionTable[index->registerNum] = functionIndex;
      return index;
    }

    GScope* createChild(bool, bool);

    void finalize();
  };
}

#endif

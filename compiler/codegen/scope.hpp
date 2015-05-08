#include <map>
#include <vector>
#include "../vm/environment.hpp"
#include "../vm/function.hpp"

#ifndef CODEGEN_SCOPE_HPP
#define CODEGEN_SCOPE_HPP

#ifdef DEBUG
  #define debug(s) std::cerr << s << std::endl;
#else
  #define debug(s);
#endif

namespace parser {
  class PFunctionDeclaration;
}

namespace codegen {

  class GScope {
  public:
    VM::GEnvironment* environment;
    GScope* parentScope;
    std::map<std::string, int> typeIndexByName;
    std::map<std::string, VM::GIndex*> localMap;
    // I think these have to be the last attributes referenced.
    // if not, this causes weird compile errors in clang.
    std::vector<VM::GFunction*> functions;
    std::vector<parser::PFunctionDeclaration*> functionDeclarations;
    bool isRootScope;

    // a lot of methods are repeated from GEnvironment.
    // they should all probably be in addObject, but
    // I want to wait to see if they actually belong here.
    VM::GIndex* addObject(std::string name, VM::GType* type) {
      if (!isRootScope) {
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

      if (parentScope != NULL) {
        return parentScope->getObject(name);
      }

      return environment->getObject(name);
    }

    int allocateFunction(VM::GFunction* function) {
      return environment->allocateFunction(function);
    }

    VM::GIndex* addClass(std::string name, VM::GType* type) {
      if (!isRootScope) {
        auto index = environment->allocateClass(type);
        typeIndexByName[name] = index->registerNum;
        return index;
      } else {
        return environment->addClass(name, type);
      }
    }

    VM::GType* getClass(std::string name) {
      if (typeIndexByName.find(name) != typeIndexByName.end()) {
        return environment->types[typeIndexByName[name]];
      }
      return environment->getClass(name);
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
      if (!isRootScope) {
        functionIndex = environment->allocateFunction(func);
      } else {
        functionIndex = environment->addFunction(name, func);
      }
      auto index = addObject(name, VM::getFunctionType());
      environment->functionTable[index->registerNum] = functionIndex;
      return index;
    }

    GScope* createChild(bool);

    void finalize();
  };
}

#endif

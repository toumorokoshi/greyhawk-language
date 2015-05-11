#include <iostream>
#include <map>
#include <vector>
#include "ops.hpp"
#include "type.hpp"
#include "object.hpp"

#ifndef VM_CONTEXT_HPP
#define VM_CONTEXT_HPP

namespace VM {

  struct GEnvironmentInstance;
  struct GFunction;

  typedef std::map<std::string, GType*> GTypeMap;

  // we use a class instead of a struct
  // so we can encapsulate things for now,
  // until a good mechanism is decided.
  class GEnvironment {
  public:
    // globals data
    std::map<std::string, int> globalsByName;
    GType** globalsTypes;
    int* indicesInParent;
    int globalsCount;

    // function data
    // std::map<std::string, GFunction*> functionByName;
    // std::map<int, int> functionTable;
    std::vector<GFunction*> functions;
    int functionCount;

    // class data
    // std::map<std::string, GType*> typeByName;
    std::vector<GType*> types;
    int typeCount;

    // locals data
    std::vector<GType*> localsTypes;
    int localsCount;

    int     allocateClass(GType* type);
    int     allocateFunction(GFunction* func);
    GIndex* allocateObject(GType* type);
    GIndex* getGlobal(std::string name);

    GEnvironmentInstance* createInstance(GEnvironmentInstance&);
    GEnvironment* createChild();
  };

  struct GEnvironmentInstance {
    GEnvironment* environment;
    GValue** globals;
    GValue* locals;
    GEnvironmentInstance* createChildScope(GEnvironmentInstance&);
  };

  GEnvironment* getEmptyEnvironment();
  GEnvironmentInstance& getEmptyEnvironmentInstance();
}
#endif

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
    std::map<std::string, int> functionsByName;
    std::vector<GFunction*> functions;
    int functionsCount;

    // class data
    std::map<std::string, int> classesByName;
    std::vector<GType*> classes;
    int classesCount;

    // locals data
    std::map<std::string, int> localsByName;
    std::vector<GType*> localsTypes;
    int localsCount;

    GIndex*     addObject(std::string name, GType* type);
    GIndex*     allocateObject(GType* type);
    GIndex*     getObject(std::string name);

    GIndex*     addClass(std::string name, GType* type);
    int         allocateClass(GType* type);
    GType*      getClass(std::string name);

    GIndex*     addFunction(std::string name, GFunction* func);
    int         allocateFunction(GFunction* func);
    GFunction*  getFunction(std::string name);


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

#include "environment.hpp"
#include <map>

#ifndef VM_FUNCTION_HPP
#define VM_FUNCTION_HPP

namespace VM {

  struct GFunctionInstance;

  typedef std::map<std::string, GEnvironmentInstance*> GModules;

  // the function is the top-level object.
  struct GFunction {
    GType* returnType;
    GInstruction* instructions;
    GEnvironment environment;
    int argumentCount;

    GFunctionInstance* createInstance(GEnvironmentInstance&);
  };

  struct GFunctionInstance {
  public:
    GFunction* function;
    GEnvironmentInstance* environmentInstance;
    GValue execute(GModules*);
  };

}

#endif

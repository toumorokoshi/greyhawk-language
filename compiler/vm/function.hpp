#include "environment.hpp"
#include <map>

#ifndef VM_FUNCTION_HPP
#define VM_FUNCTION_HPP

namespace VM {

  struct GFunctionInstance;

  typedef std::map<std::string, GEnvironmentInstance*> GModules;

  // the function is the top-level object.
  struct GFunction {
    int           argumentCount;
    std::string*  argumentNames;
    GType**       argumentTypes;
    GEnvironment*  environment;
    GInstruction* instructions;
    GType*        returnType;

    GFunctionInstance* createInstance(GEnvironmentInstance&);
  };

  struct GFunctionInstance {
  public:
    GFunction* function;
    GEnvironmentInstance& parentEnv;
    GValue execute(GModules*);
  };

}

#endif

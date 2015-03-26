#include "environment.hpp"
#include <map>

#ifndef VM_FUNCTION_HPP
#define VM_FUNCTION_HPP

namespace VM {

  class GFunction;

  typedef std::map<std::string, GEnvironmentInstance*> GModules;

  // the function is the top-level object.
  class GFunction {
  public:
    GType* returnType;
    GInstruction* instructions;
    GEnvironment scope;
    int argumentCount;

    GValue execute(GModules*, GEnvironmentInstance&);
  };

}

#endif

#include "scope.hpp"
#include <map>

#ifndef VM_FUNCTION_HPP
#define VM_FUNCTION_HPP

namespace VM {

  class GFunction;

  typedef std::map<std::string, GScopeInstance*> GModules;

  // the function is the top-level object.
  class GFunction {
  public:
    GType* returnType;
    GInstruction* instructions;
    GScope* scope;
    int argumentCount;

    GObject execute(GModules*, GScopeInstance&);
  };

}

#endif

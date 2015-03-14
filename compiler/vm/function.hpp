#include "scope.hpp"
#include <map>

#ifndef VM_FUNCTION_HPP
#define VM_FUNCTION_HPP

namespace VM {

  class GFunction;

  typedef std::map<std::string, GFunction> GModules;

  // the function is the top-level object.
  class GFunction {
  public:
    GType* returnType;
    GInstruction* instructions;
    G2Scope* scope;
    int argumentCount;

    GValue* execute(GModules*, G2ScopeInstance&);
  };

}

#endif

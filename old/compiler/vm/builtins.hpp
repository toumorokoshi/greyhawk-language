/*
  builtins are methods that are built in c and
  exposed to Greyhawk.
 */
#include <vector>
#include <functional>
#include "object.hpp"

#ifndef VM_BUILTINS_HPP
#define VM_BUILTINS_HPP

namespace VM {
  extern std::map<std::string, Builtin> builtinMap;
  extern std::vector<Builtin> builtinList;

  GValue* builtin_read(GValue* args);
  GType* getBuiltinModuleType();
  GEnvironmentInstance* getBuiltins();
}


#endif

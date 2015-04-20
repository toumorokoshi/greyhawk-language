/*
  builtins are methods that are built in c and
  exposed to Greyhawk.
 */
#include <vector>
#include <functional>
#include "object.hpp"

namespace VM {
  typedef std::function<GValue*(GValue*)> Builtin;
  extern std::map<std::string, Builtin> builtinMap;
  extern std::vector<Builtin> builtinList;

  GValue* builtin_read(GValue* args);
}

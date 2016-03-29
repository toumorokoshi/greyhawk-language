#include "../type.hpp"
#include "../object.hpp"
#include <map>

namespace VM {

  typedef std::map<std::string, PrimitiveMethodMap> PrimitiveMap;
  extern PrimitiveMap primitives;
}

#include "primitives.hpp"
#include "array.hpp"


namespace VM {

  PrimitiveMap primitives = {
    {"Array", arrayMethods}
  };
}

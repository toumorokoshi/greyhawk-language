#include "string.hpp"

namespace VM {

  GType* getStringType() {
    return getArrayType(getCharType());
  }
}

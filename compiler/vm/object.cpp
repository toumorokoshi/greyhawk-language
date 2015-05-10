#include "object.hpp"

namespace VM {

  GValue* getNoneObject() {
    auto noneObject = new GValue{ 0 };
    return noneObject;
  }
}

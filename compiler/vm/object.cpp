#include "object.hpp"

namespace VM {

  GObject* getNoneObject() {
    auto noneObject = new GObject { getNoneType(),  {0}};
    return noneObject;
  }
}

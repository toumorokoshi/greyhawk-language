#include "class.hpp"

namespace VM {

  VMClass* getNoneType() {
    static VMClass* None = new VMClass("None");
    return None;
  }
}

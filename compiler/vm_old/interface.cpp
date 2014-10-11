#include "./interface.hpp"

namespace VM {

  VMInterface* getVMIStringable() {
    static VMInterface* vmStringable = new VMInterface("Stringable");
    return vmStringable;
  }

  VMInterface* getVMIIterable() {
    static VMInterface* vmIterable = new VMInterface("Iterable");
    return vmIterable;
  }
}

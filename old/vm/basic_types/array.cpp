#include "array.hpp"

namespace VM {

  VMClass* getVMArrayClass() {
    static auto VMArrayClass = new VMClass("Array");
    return VMArrayClass;
  }
}

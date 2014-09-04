#include "./class.hpp"

namespace VM {

  VMClass* getVMStringClass() {
    static auto VMStringClass = new VMClass();
    return VMStringClass;
  }

  VMClass* getVMMethodClass() {
    static auto VMMethodClass = new VMClass();
    return VMMethodClass;
  }
}

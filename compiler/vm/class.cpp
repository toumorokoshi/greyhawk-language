#include "./class.hpp"

namespace VM {

  VMClass* getVMFunctionClass() {
    static auto VMFunctionClass = new VMClass("Function");
    return VMFunctionClass;
  }

  VMClass* getVMMethodClass () {
    static auto VMMethodClass = new VMClass("Method");
    return VMMethodClass;
  }
}

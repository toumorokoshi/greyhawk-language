#include "int.hpp"
#include "../method.hpp"
#include "string.hpp"
#include <iostream>

namespace VM {

  VMObject* InttoString(VMObject* self, VMObjectList& arguments) {
    auto integer = dynamic_cast<VMInt*>(self);
    return new VMString(std::to_string(integer->value));
  }

  VMClass* getVMIntClass() {
    static auto VMIntClass = new VMClass("Int");
    static bool _initialized = false;

    if (!_initialized) {
      auto arguments = new std::vector<VMClass*>;
      VMIntClass->methods["toString"] = new VMMethod(*arguments,
                                                        (VMRawMethod) &InttoString);
    }

    return VMIntClass;
  }

}

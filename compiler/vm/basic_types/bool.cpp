#include "bool.hpp"

namespace VM {

  VMObject* BoolToString(VMObject* self, VMObjectList& arguments) {
    auto boolObject = dynamic_cast<VMBool*>(self);
    return new VMString(boolObject->value ? "true" : "false");
  }

  VMClass* getVMBoolClass() {
    static auto VMBoolClass = new VMClass("Bool");
    static bool _initialized = false;

    if (!_initialized) {
      auto arguments = new std::vector<VMClass*>;
      VMBoolClass->methods["toString"] = new VMMethod(*arguments,
                                                      (VMRawMethod) &BoolToString);
    }

    return VMBoolClass;
  }
}

#include "string.hpp"
#include "../method.hpp"

namespace VM {

  VMObject* toString(VMObject* self, VMObjectList& arguments) {
    return self;
  }

  VMClass* getVMStringClass() {
    static auto VMStringClass = new VMClass("String");
    static bool _initialized = false;

    if (!_initialized) {
      auto arguments = new std::vector<VMClass*>;
      VMStringClass->methods["toString"] = new VMMethod(*arguments,
                                                        (VMRawMethod) &toString);
    }

    return VMStringClass;
  }

}

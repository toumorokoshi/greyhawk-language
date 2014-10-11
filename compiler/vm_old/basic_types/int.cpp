#include "int.hpp"
#include "../method.hpp"
#include "../function.hpp"
#include "string.hpp"
#include <string>
#include <iostream>

namespace VM {

  VMObject* StringToInt(VMObjectList& arguments) {
    auto stringObject = dynamic_cast<VMString*>(arguments[0]);
    return new VMInt(std::stoi(stringObject->value));
  }

  VMFunction* _getVMIntConstructor() {
    auto static VMIntConstructor = new VMRawFunctionWrapper(getVMIntClass(),
                                                            *new std::vector<VMClass*>{ getVMStringClass() },
                                                            (VMRawFunction) StringToInt
                                                            );
    return VMIntConstructor;
  }

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

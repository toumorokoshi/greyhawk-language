#include "./class.hpp"
#include "./builtins.hpp"
#include "./exceptions.hpp"
#include <iostream>

namespace VM {

  VMObject* vm_print(std::vector<VMObject*>& args) {
    auto string = dynamic_cast<VMString*>(*args.begin());
    std::cout << string->value << std::endl;
    return NULL;
  }

  VMMethod& _getVMPrint() {
    auto argumentTypes = new std::vector<VMClass*>();
    argumentTypes->push_back(getVMStringClass());

    return *new VMMethod(*argumentTypes, (VMRawMethod) &vm_print);
  }

  VMScope& getBuiltinScope() {
    static VMScope _BUILTIN_SCOPE;
    static bool _initialized = false;
    if (!_initialized) {
      _BUILTIN_SCOPE.locals["print"] = &_getVMPrint();
      _initialized = true;
    }
    return _BUILTIN_SCOPE;
  }
}

#include "./class.hpp"
#include "./builtins.hpp"
#include "./exceptions.hpp"
#include "./interface.hpp"
#include "./arithmetic.hpp"
#include <iostream>

namespace VM {

  VMObject* vm_print(std::vector<VMObject*>& args) {
    auto string =
      dynamic_cast<VMString*>((*args.begin())->call("toString",
                                                    *new std::vector<VMObject*>()));
    std::cout << string->value << std::endl;
    return NULL;
  }

  VMFunction& _getVMPrint() {
    auto argumentTypes = new std::vector<VMClass*>();
    argumentTypes->push_back(getVMIStringable());

    return *new VMFunction(*argumentTypes, (VMRawFunction) &vm_print);
  }

  VMScope& getBuiltinScope() {
    static VMScope _BUILTIN_SCOPE;
    static bool _initialized = false;
    if (!_initialized) {
      _BUILTIN_SCOPE.locals["print"] = &_getVMPrint();
      _BUILTIN_SCOPE.locals["__add"] = &_getVMAdd();
      _BUILTIN_SCOPE.locals["__sub"] = &_getVMSub();
      _BUILTIN_SCOPE.locals["__mul"] = &_getVMMul();
      _BUILTIN_SCOPE.locals["__div"] = &_getVMDiv();
      _initialized = true;
    }
    return _BUILTIN_SCOPE;
  }
}

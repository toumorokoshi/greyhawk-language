#include "./class.hpp"
#include "./builtins.hpp"
#include "./exceptions.hpp"

namespace VM {

  VMObject* vm_print(std::vector<VMObject*>& objects) {
    auto string = dynamic_cast<VMString*>(*objects.begin());
    printf(string->value.c_str());
  }

  VMMethod& _getVMPrint() {
    auto argumentTypes = new std::vector<VMClass*>();
    argumentTypes->push_back(getVMStringClass());

    VMRawMethod _vm_print = [] (std::vector<VMObject*>& objects) -> VMObject* {
      auto string = dynamic_cast<VMString*>(*objects.begin());
      printf(string->value.c_str());
    };

    return new VMMethod(*argumentTypes, _vm_print);
  }

  VMScope* _BUILTIN_SCOPE = NULL;

  VMScope& getBuiltinScope() {
    if (_BUILTIN_SCOPE == NULL) {
      _BUILTIN_SCOPE = new VMScope();
      _BUILTIN_SCOPE->locals["print"] = &_getVMPrint();
    }
    return *_BUILTIN_SCOPE;
  }
}

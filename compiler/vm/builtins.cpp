#include "./class.hpp"
#include "./builtins.hpp"
#include "./exceptions.hpp"

namespace VM {

  void vm_print(std::vector<VMObject*> objects) {
    auto string = dynamic_cast<VMString*>(*objects.begin());
    printf(string->value.c_str());
  }

  VMMethod& _getVMPrint() {
    auto argumentTypes = new std::vector<VMClass*>();
    argumentTypes->push_back(&VMStringClass);
    return new VMMethod(argumentTypes, vm_print);
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

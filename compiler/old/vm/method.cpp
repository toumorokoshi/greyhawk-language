#include "method.hpp"

namespace VM {
  VMClass* getVMMethodClass () {
    static auto VMMethodClass = new VMClass("Method");
    return VMMethodClass;
  }

  VMMethod::VMMethod(std::vector<VMClass*>& argumentTypes, VMRawMethod rawMethod) :
    _argumentTypes(argumentTypes), _rawMethod(rawMethod) {}

  VMObject* VMMethod::call(VMObject* self,
                           std::vector<VMObject*>& arguments) {

    // type checking
    if (arguments.size() != _argumentTypes.size()) {
      throw VMException("mismatched argument count!");
    }

    for (int i = 0; i < arguments.size(); i++) {
      if (!_argumentTypes[i]->matches(arguments[i]->getType())) {
        throw VMException("type difference!");
      }
    }

    return _rawMethod(self, arguments);
  }
}

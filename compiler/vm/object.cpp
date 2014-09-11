#include "./object.hpp"
#include "./exceptions.hpp"
#include <iostream>

namespace VM {

  VMObject* VMObject::call(std::string methodName, VMObjectList& args) {
      VMClass* type = getType();
      if (type->methods.find(methodName) == type->methods.end()) {
        throw VMException("unable to find method!");
      }
      return type->methods[methodName]->call(this, args);
  }

  VMObject* VMFunction::call(VMObjectList& arguments) {
    // type checking
    if (arguments.size() != _argumentTypes.size()) {
      throw VMException("mismatched argument count!");
    }

    for (int i = 0; i < arguments.size(); i++) {
      if (!_argumentTypes[i]->matches(arguments[i]->getType())) {
        throw VMException("type difference!");
      }
    }

    return _rawFunction(arguments);
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

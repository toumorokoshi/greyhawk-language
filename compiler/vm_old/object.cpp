#include "object.hpp"
#include "exceptions.hpp"
#include "method.hpp"
#include <iostream>

namespace VM {

  VMObject* VMObject::call(std::string methodName, VMObjectList& args) {
      VMClass* type = getType();
      if (type->methods.find(methodName) == type->methods.end()) {
        throw VMException("unable to find method!");
      }
      return type->methods[methodName]->call(this, args);
  }
}

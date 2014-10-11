#include "function.hpp"
#include <iostream>

namespace VM {

  VMClass* getVMFunctionClass() {
    static auto VMFunctionClass = new VMClass("Function");
    return VMFunctionClass;
  }

  VMObject* VMRawFunctionWrapper::call(VMScope& scope, VMObjectList& arguments) {
    return _rawFunction(arguments);
  }

  VMObject* VMFunctionDeclaration::call(VMScope& scope, VMObjectList& objects) {

    VMScope localScope(scope);

    // type checking
    if (objects.size() != arguments.size()) {
      throw VMException("mismatched argument count!");
    }

    for (int i = 0; i < arguments.size(); i++) {
      auto argumentDefinition = arguments[i];
      auto object = objects[i];
      /* if (object[i].getType() != argumentDefinition.second) {
        throw VMException("type difference!");
        } */

      localScope.locals[argumentDefinition->first] = object;
    }

    return body->execute(localScope);
  }

  VMObject* VMFunctionDeclaration::execute(VMScope& scope) {
    scope.locals[functionName] = this;
    return NULL;
  }

}

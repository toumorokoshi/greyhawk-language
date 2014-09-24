#include "function.hpp"

namespace VM {

  VMObject* VMRawFunctionWrapper::call(VMScope& scope, VMObjectList& arguments) {
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

    body->execute(localScope);
    return NULL;
  }

  void VMFunctionDeclaration::execute(VMScope& scope) {
    scope.locals[functionName] = this;
  }

}

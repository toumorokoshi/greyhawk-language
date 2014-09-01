#include "./object.hpp"

namespace VM {

  VMObject* VMMethod::call(VMScope& parentScope, std::vector<VMObject*>& arguments) {
    // type checking
    if (arguments.size() != method->argumentTypes.size()) {
      throw VMException(methodName + " requires " +
                        arguments.size() + " arguments. " +
                        method->argumentTypes.size() + " arguments were passed.");
    }

    for (int i = 0; i < arguments.size(); i++) {
      if (arguments[i].getType() != method->argumentTypes[i]) {
        throw VMException("type difference!");
      }
    }

    // create a scope
    auto methodScope = new VMScope(&parentScope);

    methodScope.executeStatement(
    return method.call(arguments);
  }
}

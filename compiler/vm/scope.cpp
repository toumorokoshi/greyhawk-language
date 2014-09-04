#include "./scope.hpp"
#include "./exceptions.hpp"

namespace VM {

  VMObject* VMScope::invokeMethod(std::string methodName,
                                  std::vector<VMObject*>& arguments) {
    if (locals.find(methodName) == locals.end()) {
      throw VMException(methodName + " is not defined in this scope!");
    }

    auto object = locals[methodName];
    if (auto method = dynamic_cast<VMMethod*>(object)) {
      return method->call(arguments);

    } else {
      throw VMException(methodName + " is not a method!");
    }
  }
}

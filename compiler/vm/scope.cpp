#include "./scope.hpp"
#include "./exceptions.hpp"
#include "function.hpp"

namespace VM {

  VMObject* VMScope::invokeMethod(std::string methodName,
                                  std::vector<VMObject*>& arguments) {
    auto object = getObject(methodName);

    if (auto method = dynamic_cast<VMFunction*>(object)) {
      return method->call(*this, arguments);

    } else {
      throw VMException(methodName + " is not a function!");
    }
  }
}

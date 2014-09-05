#include "./object.hpp"
#include "./exceptions.hpp"
#include <iostream>

namespace VM {

  VMObject* VMMethod::call(std::vector<VMObject*>& arguments) {
    // type checking
    if (arguments.size() != _argumentTypes.size()) {
      throw VMException("mismatched argument count!");
    }

    for (int i = 0; i < arguments.size(); i++) {
      if (arguments[i]->getType() != _argumentTypes[i]) {
        throw VMException("type difference!");
      }
    }

    return _rawMethod(arguments);
  }
}

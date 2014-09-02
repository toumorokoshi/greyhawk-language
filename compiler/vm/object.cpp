#include "./object.hpp"
#include "./exceptions.hpp"

namespace VM {

  VMObject* VMMethod::call(std::vector<VMObject*>& arguments) {
    // type checking
    if (arguments.size() != _argumentTypes.size()) {
      throw VMException("method requires " + arguments.size() + " arguments. " +
                        _argumentTypes.size() + " arguments were passed.");
    }

    for (int i = 0; i < arguments.size(); i++) {
      if (arguments[i]->getType() != _argumentTypes[i]) {
        throw VMException("type difference!");
      }
    }

    return _rawMethod(arguments);
  }
}

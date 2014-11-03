#include "scope.hpp"

namespace VM {

  GFunction* GScope::getFunction(std::string name) {
    if (functionTable.find(name) != functionTable.end()) {
      return functionTable[name];
    } else if (_parent != NULL) {
      return _parent->getFunction(name);
    }
    return NULL;
  }

}

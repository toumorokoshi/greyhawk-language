#include "scope.hpp"

namespace VM {

  GOldFunction* GScope::getFunction(std::string name) {
    if (functionTable.find(name) != functionTable.end()) {
      return functionTable[name];
    } else if (_parent != NULL) {
      return _parent->getFunction(name);
    }
    return NULL;
  }

  void G2Scope::addLocal(std::string name) {
    locals[name] = registerCount++;
  }

}

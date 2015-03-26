#include "../vm/environment.hpp"

namespace codegen {

  class GScope {
  public:
    VM::GEnvironment* environment;

    // a lot of methods are repeated from GEnvironment.
    // they should all probably be in addObject, but
    // I want to wait to see if they actually belong here.
    VM::GIndex* addObject(std::string name, VM::GType* type) {
      return environment->addObject(name, type);
    }

    VM::GIndex* allocateObject(VM::GType* type) {
      return environment->allocateObject(type);
    }

    VM::GIndex* getObject(std::string name) {
      return environment->getObject(name);
    }
  };
}

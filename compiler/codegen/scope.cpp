#include "scope.hpp"

using namespace VM;

namespace codegen {

  GScope* GScope::createChild(bool withNewEnvironment, bool isInnerScope=false) {
    GEnvironment* childEnvironment;
    if (withNewEnvironment) {
      childEnvironment = environment->createChild();
    } else {
      childEnvironment = environment;
    }

    return new GScope {
      .environment = childEnvironment,
      .isInnerScope = isInnerScope
    };
  }
}

#include "scope.hpp"
#include "../parser/nodes.hpp"

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

  void GScope::finalize() {
    for (int i = 0; i < functions.size(); i++) {
      auto function = functions[i];
      auto declaration = functionDeclarations[i];
      declaration->generateBody(function, this);
    }
  }
}

#include "scope.hpp"
#include "../parser/nodes.hpp"

using namespace VM;

namespace codegen {

  /* GScope* GScope::createChild2(bool withNewEnvironment,
                              bool isRootScope=true,
                              bool isInnerScope=false) {
    GEnvironment* childEnvironment;
    if (withNewEnvironment) {
      childEnvironment = environment->createChild();
    } else {
      childEnvironment = environment;
    }

    GScope* parentScope = NULL;
    if (isInnerScope) {
      parentScope = this;
    }

    return new GScope {
      .environment = childEnvironment,
      .parentScope = parentScope,
      .isRootScope = isRootScope,
    };
    } */

  GScope* GScope::createChild(bool isRootScope) {
    GEnvironment* childEnvironment;
    if (isRootScope) {
      childEnvironment = environment->createChild();
    } else {
      childEnvironment = environment;
    }

    GScope* parentScope = NULL;
    if (!isRootScope) {
      parentScope = this;
    }

    return new GScope {
      .environment = childEnvironment,
      .parentScope = parentScope,
      .isRootScope = isRootScope,
    };
  }



  void GScope::finalize() {
    for (int i = 0; i < (int) functions.size(); i++) {
      auto function = functions[i];
      auto declaration = functionDeclarations[i];
      declaration->generateBody(function, this);
    }
  }
}

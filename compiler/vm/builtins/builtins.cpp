#include "builtins.hpp"

namespace VM {

  GScope* getGlobalScope() {
    static auto globalScope = new GScope(NULL, NULL);
    static auto _initialized = false;
    if (!_initialized) {
      _initialized = true;
    }
    return globalScope;
  }
};

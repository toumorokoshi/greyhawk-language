#include "system.hpp"
#include <map>
#include <stdio.h>

namespace VM {
  GModule* getSystemModule() {
    static auto systemModule = new GModule();
    static auto _initialized = false;
    if (!_initialized) {
      // systemModule.
      // _initialized = true;
    }
    return systemModule;
  }
}

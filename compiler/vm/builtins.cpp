#include "builtins.hpp"
#include "environment.hpp"
// unistd is a unix-specific thing.
// we'll need to do an if/else for windows at
// some point.
#include <unistd.h>

namespace VM {

  // None __builtin__.read(fd Int, buffer Array<char>, size Int)
  GValue* builtin_read(GValue* args) {
    int fd = args[0].asInt32;
    GValue* buffer = (GValue*) args[1].asArray->elements;
    int size = args[2].asInt32;

    // TODO: this is really messy. this
    // should use the buffer directly at some point
    // in the future.
    char inBuf[size];
    read(fd, &inBuf, size);

    for (int i = 0; i < size; i++) {
      buffer[i].asChar = inBuf[i];
    }

    return getNoneObject();
  }

  /* std::map<std::string, Builtin> builtinMap = {
    {"read", builtin_read}
    }; */

  GType* getBuiltinModuleType() {
    auto static _initialized = false;
    auto static builtinEnv = new GEnvironment();
    auto static builtinType = new GType {
      "Builtin", NULL,
      .environment = builtinEnv
    };
    if (!_initialized) {
      builtinEnv->addObject("read", getBuiltinType());
      _initialized = true;
    }
    return builtinType;
  }

  GEnvironmentInstance* getBuiltins() {
    auto static _initialized = false;
    auto static environment =                                         \
      getBuiltinModuleType()->environment->createInstance(getEmptyEnvironmentInstance());
    if (!_initialized) {
      environment->locals[0].asBuiltin = &builtin_read;
      _initialized = true;
    }
    return environment;
  }
}

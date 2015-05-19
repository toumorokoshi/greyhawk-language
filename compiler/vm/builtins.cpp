#include "builtins.hpp"
#include "environment.hpp"
#include "function.hpp"
// unistd is a unix-specific thing.
// we'll need to do an if/else for windows at
// some point.
#include <unistd.h>

namespace VM {

  void addBuiltinRead(GEnvironment* environment) {
    environment->addFunction("read", new GFunction {
        .argumentCount = 3,
        .argumentTypes = new GType*[3]{
          getInt32Type(),
          getStringType(),
          getInt32Type()
        },
        .returnType = getInt32Type()
    });
    environment->localsTypes[environment->localsCount - 1] = getBuiltinType();
  }

  // None __builtin__.read(fd Int, buffer Array<char>, size Int)
  GValue* builtin_read(GValue* args) {
    int fd = args[0].asInt32;
    GValue* buffer = (GValue*) args[1].asArray->elements;
    int size = args[2].asInt32;

    // TODO: this is really messy. this
    // should use the buffer directly at some point
    // in the future.
    char inBuf[size];
    int bytesRead = read(fd, &inBuf, size);

    for (int i = 0; i < size; i++) {
      buffer[i].asChar = inBuf[i];
    }

    return new GValue { bytesRead };
  }

  void addBuiltinWrite(GEnvironment* environment) {
    environment->addFunction("write", new GFunction {
        .argumentCount = 3,
        .argumentTypes = new GType*[3]{
          getInt32Type(),
          getStringType(),
          getInt32Type()
        },
        .returnType = getNoneType()
    });
    environment->localsTypes[environment->localsCount - 1] = getBuiltinType();
  }

  GValue* builtin_write(GValue* args) {
    // TODO: this should return the integer value.
    int fd = args[0].asInt32;
    GValue* buffer = (GValue*) args[1].asArray->elements;
    int size = args[2].asInt32;

    // TODO: this is really messy. this
    // should use the buffer directly at some point
    // in the future.
    char inBuf[size];

    for (int i = 0; i < size; i++) {
      inBuf[i] = buffer[i].asChar;
    }

    write(fd, &inBuf, size);

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
      addBuiltinRead(builtinEnv);
      addBuiltinWrite(builtinEnv);
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
      environment->locals[1].asBuiltin = &builtin_write;
      _initialized = true;
    }
    return environment;
  }
}

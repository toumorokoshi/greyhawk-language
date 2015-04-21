#include "type.hpp"
#include "environment.hpp"
#include "function.hpp"
#include <map>

namespace VM {

  GType* getArrayType(GType* elementType) {
    static std::map<GType*, GType*> arrayTypes;
    if (arrayTypes.find(elementType) == arrayTypes.end()) {
      arrayTypes[elementType] = new GType {
        .name = "Array",
        .subTypes = new GType*[1] { elementType }};
    }
    return arrayTypes[elementType];
  }

  GType* getBoolType() {
    auto static boolType = new GType { "Bool", NULL };
    return boolType;
  }

  GType* getBuiltinType() {
    auto static builtinType = new GType { "Builtin", NULL };
    return builtinType;
  }

  GType* getClassType() {
    auto static classType = new GType { "Class", NULL };
    return classType;
  };

  GType* getCharType() {
    auto static charType = new GType { "Char", NULL };
    return charType;
  };

  GType* getFloatType() {
    auto static floatType = new GType { "Float", NULL };
    return floatType;
  }

  GType* getFunctionType() {
    auto static functionType = new GType { "Function", NULL };
    return functionType;
  }

  GType* getInt32Type() {
    auto static intType = new GType { "Int32", NULL };
    return intType;
  }

  GType* getModuleType() {
    auto static moduleType = new GType { "Module", NULL };
    return moduleType;
  }

  GType* getFileHandleType() {
    auto static fileHandleType = new GType { "FileHandle", NULL };
    return fileHandleType;
  }

  GType* getStringType() {
    return getArrayType(getCharType());
  }

  GType* getNoneType() {
    auto static noneType = new GType { "None", NULL };
    return noneType;
  }

  GEnvironmentInstance* GType::instantiate() {
    auto instance = environment->createInstance(*parentEnv);
    // we instantiate all the methods, binding them to the current context.
    for (int i = 0; i < functionCount; i++) {
      // methods are instantiate after type.
      int methodIndex = i + attributeCount;
      instance->locals[methodIndex].asFunction = new GFunctionInstance {
        .function = environment->functions[i],
        .parentEnv = *instance
      };
    }
    return instance;
  }
}

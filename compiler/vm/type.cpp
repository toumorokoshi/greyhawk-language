#include "type.hpp"
#include "environment.hpp"
#include "function.hpp"
#include <sstream>
#include <map>

namespace VM {

  GType* getArrayType(GType* elementType) {
    static std::map<GType*, GType*> arrayTypes;
    if (arrayTypes.find(elementType) == arrayTypes.end()) {
      arrayTypes[elementType] = new GType {
        .name = "Array",
        .subTypes = gstd::Array<GType*> (new GType*[1] { elementType }, 1)
      };
    }
    return arrayTypes[elementType];
  }

  GType* getBoolType() {
    auto static boolType = new GType {
      "Bool", gstd::Array<GType*> (NULL, 0)
    };
    return boolType;
  }

  GType* getBuiltinType() {
    auto static builtinType = new GType {
      "Builtin", gstd::Array<GType*> (NULL, 0)
    };
    return builtinType;
  }

  GType* getClassType() {
    auto static classType = new GType {
      "Class", gstd::Array<GType*> (NULL, 0)
    };
    return classType;
  };

  GType* getCharType() {
    auto static charType = new GType {
      "Char", gstd::Array<GType*> (NULL, 0)
    };
    return charType;
  };

  GType* getFloatType() {
    auto static floatType = new GType {
      "Float", gstd::Array<GType*> (NULL, 0)
    };
    return floatType;
  }

  GType* getFunctionType() {
    auto static functionType = new GType {
      "Function", gstd::Array<GType*> (NULL, 0)
    };
    return functionType;
  }

  GType* getInt32Type() {
    auto static intType = new GType {
      "Int32", gstd::Array<GType*> (NULL, 0)
    };
    return intType;
  }

  GType* getModuleType() {
    auto static moduleType = new GType {
      "Module", gstd::Array<GType*> (NULL, 0)
    };
    return moduleType;
  }

  GType* getFileHandleType() {
    auto static fileHandleType = new GType {
      "FileHandle", gstd::Array<GType*> (NULL, 0)
    };
    return fileHandleType;
  }

  GType* getStringType() {
    return getArrayType(getCharType());
  }

  GType* getNoneType() {
    auto static noneType = new GType {
      "None", gstd::Array<GType*> (NULL, 0)
    };
    return noneType;
  }

  GType* getTupleType(gstd::Array<GType*> subTypes) {
    static std::map<std::string, GType*> tupleTypes;
    std::stringstream typeStream;
    typeStream << "Tuple<";
    for (int i = 0; i < subTypes.length(); i++) {
      typeStream << subTypes[i]->name;
      if (i + 1 < subTypes.length()) { typeStream << ","; }
    }
    typeStream << ">";
    auto name = typeStream.str();

    if (tupleTypes.find(name) == tupleTypes.end()) {
      tupleTypes[name] = new GType {
        .name = name,
        .subTypes = subTypes
      };
    }

    return tupleTypes[name];
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

#include "type.hpp"
#include "environment.hpp"
#include "function.hpp"
#include <sstream>
#include <map>

using gstd::Array;

namespace VM {

  GType* getBoolType() {
    auto static boolType = new GType {
      "Bool", Array<GType*>(0),
      .isPrimitive = true
    };
    return boolType;
  }

  GType* getBuiltinType() {
    auto static builtinType = new GType {
      "Builtin", Array<GType*>(0)
    };
    return builtinType;
  }

  GType* getClassType() {
    auto static classType = new GType {
      "Class", Array<GType*>(0),
      .isPrimitive = true
    };
    return classType;
  };

  GType* getCharType() {
    auto static charType = new GType {
      "Char", Array<GType*>(0),
      .isPrimitive = true
    };
    return charType;
  };

  GType* getFloatType() {
    auto static floatType = new GType {
      "Float", Array<GType*>(0),
      .isPrimitive = true
    };
    return floatType;
  }

  GType* getFunctionType() {
    auto static functionType = new GType {
      "Function", Array<GType*>(0),
      .isPrimitive = true
    };
    return functionType;
  }

  GType* getInt32Type() {
    auto static intType = new GType {
      "Int32", Array<GType*>(0),
      .isPrimitive = true
    };
    return intType;
  }

  GType* getModuleType() {
    auto static moduleType = new GType {
      "Module", Array<GType*>(0),
      .isPrimitive = true
    };
    return moduleType;
  }

  GType* getNoneType() {
    auto static noneType = new GType {
      "None", Array<GType*>(0),
      .isPrimitive = true
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

  bool isTupleType(GType* type) {
    return type->name.find("Tuple<") != std::string::npos;
  }
}

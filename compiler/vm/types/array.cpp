#include <map>
#include "array.hpp"

using gstd::Array;

namespace VM {

  GType* getArrayType(GType* elementType) {
    static std::map<GType*, GType*> arrayTypes;
    if (arrayTypes.find(elementType) == arrayTypes.end()) {
      arrayTypes[elementType] = new GType {
        .name = "Array",
        .subTypes = Array<GType*> (new GType*[1] { elementType }, 1),
        .isPrimitive = true,
      };
    }
    return arrayTypes[elementType];
  }

  bool isArrayType(GType* type) {
    return type->name.find("Array") != std::string::npos;
  }

  GValue arraySize(GValue array, GValue*) {
    return {array.asArray->size};
  }

  PrimitiveMethodMap arrayMethods = {
    {"size", { getInt32Type(), &arraySize}}
  };
}

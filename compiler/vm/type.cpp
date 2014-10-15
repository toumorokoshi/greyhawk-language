#include "type.hpp"

namespace VM {

  GType* getArrayType() {
    auto static arrayType = new GType { ARRAY, "Array" };
    return arrayType;
  }

  GType* getBoolType() {
    auto static boolType = new GType { BOOL, "Bool" };
    return boolType;
  }

  GType* getInt32Type() {
    auto static intType = new GType { INT32, "Int32" };
    return intType;
  }

  GType* getStringType() {
    auto static stringType = new GType { STRING, "String" };
    return stringType;
  }

  GType* getNoneType() {
    auto static noneType = new GType { NONE, "None" };
    return noneType;
  }
}

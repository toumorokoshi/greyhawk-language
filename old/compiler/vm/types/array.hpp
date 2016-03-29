#include "../type.hpp"
#include "../object.hpp"

#ifndef VM_ARRAY_HPP
#define VM_ARRAY_HPP

namespace VM {
  GType* getArrayType(GType* elementType);
  bool isArrayType(GType*);

  extern PrimitiveMethodMap arrayMethods;
}

#endif

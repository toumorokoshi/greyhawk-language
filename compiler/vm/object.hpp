#include "type.hpp"

#ifndef VM2_VALUE_HPP
#define VM2_VALUE_HPP

namespace VM {

  union GValue;

  typedef struct {
    GValue* elements;
    int size;
  } GArray;

  typedef union GValue {
    int asInt32;
    bool asBool;
    double asFloat;
    const char* asString;
    void* asNone;
    GArray* asArray;
  } GValue;

  typedef struct GObject {
    GType* type;
    int registerNum;
  } GObject;

  GObject* getNoneObject();
}

#endif

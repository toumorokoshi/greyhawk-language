#include "type.hpp"

#ifndef VM2_VALUE_HPP
#define VM2_VALUE_HPP

namespace VM {

  struct GObject;

  typedef struct {
    GObject** elements;
    int size;
  } GArray;

  typedef union {
    int asInt32;
    bool asBool;
    double asFloat;
    char* asString;
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

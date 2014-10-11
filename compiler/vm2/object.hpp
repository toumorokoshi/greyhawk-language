#include "type.hpp"

#ifndef VM2_VALUE_HPP
#define VM2_VALUE_HPP

namespace VM {

  typedef union {
    int asInt32;
    bool asBool;
    char* asString;
    void* asNone;
  } GValue;

  typedef struct {
    GType* type;
    GValue value;
  } GObject;
}

#endif

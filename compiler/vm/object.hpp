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
    char asChar;
    double asFloat;
    void* asNone;
    GArray* asArray;
  } GValue;

  typedef struct GObject {
    GType* type;
    int registerNum;
  } GObject;

  GObject* getNoneObject();

  std::string getValueDebugInfo(GValue v);
}

#endif

#include "type.hpp"
#include <map>

#ifndef VM2_VALUE_HPP
#define VM2_VALUE_HPP

namespace VM {

  union GValue;
  struct GScopeInstance;

  typedef struct {
    GValue* elements;
    int size;
  } GArray;

  // this represents an instance of a class.
  // all it needs are explicit set of values for each
  // attribute it has.
  typedef struct {
    GType* type;
    GValue* attributes;
  } GInstance;

  typedef union GValue {
    int asInt32;
    bool asBool;
    char asChar;
    double asFloat;
    void* asNone;
    GArray* asArray;
    GScopeInstance* asModule;
    GInstance* asInstance;
    GType* asType;
    FILE* asFile;
  } GValue;

  typedef struct GObject {
    GType* type;
    GValue value;
  } GObject;

  typedef struct GIndex {
    int registerNum;
    GType* type;
  } GIndex;

  /*typedef struct GObject {
    GType* type;
    int registerNum;
  } GObject; */

  GObject* getNoneObject();

  std::string getValueDebugInfo(GValue v);
}

#endif

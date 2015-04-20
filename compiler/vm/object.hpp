#include "type.hpp"
#include <map>

#ifndef VM2_VALUE_HPP
#define VM2_VALUE_HPP

namespace VM {

  struct GArray;
  struct GObject;
  struct GEnvironmentInstance;
  struct GFunctionInstance;


  typedef union GValue {
    int asInt32;
    bool asBool;
    char asChar;
    double asFloat;
    void* asNone;
    GArray* asArray;
    GEnvironmentInstance* asModule;
    GEnvironmentInstance* asInstance;
    GFunctionInstance* asFunction;
    GType* asType;
    FILE* asFile;
  } GValue;

  typedef struct GArray {
    GValue* elements;
    int size;
  } GArray;

  typedef struct GObject {
    GType* type;
    GValue value;
  } GObject;

  enum GIndexType {
    LOCAL,
    GLOBAL,
    OBJECT_PROPERTY
  };

  struct GIndex;

  typedef struct GIndex {
    GIndexType indexType;
    GIndex* objectIndex;
    int registerNum;
    GType* type;
  } GIndex;

  /*typedef struct GObject {
    GType* type;
    int registerNum;
  } GObject; */

  GValue* getNoneObject();

  std::string getValueDebugInfo(GValue v);
}

#endif

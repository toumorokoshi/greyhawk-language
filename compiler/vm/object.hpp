#include "type.hpp"
#include <map>

#ifndef VM2_VALUE_HPP
#define VM2_VALUE_HPP

namespace VM {

  union GValue;
  struct GModule;

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
    GModule* asModule;
    GValue* asClassInstance;
    FILE* asFile;
  } GValue;

  typedef struct GObject {
    GType* type;
    int registerNum;
  } GObject;

  typedef struct GModule {
    std::map<std::string, GValue> globals;
  } GModule;

  GObject* getNoneObject();

  std::string getValueDebugInfo(GValue v);
}

#endif

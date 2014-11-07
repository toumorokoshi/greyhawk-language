#include <string>

#ifndef VM2_TYPE_HPP
#define VM2_TYPE_HPP

namespace VM {

  enum BASICTYPES {
    ARRAY,
    BOOL,
    CHAR,
    CLASS,
    FLOAT,
    INT32,
    NONE
  };

  struct GType;

  typedef struct GType {
    BASICTYPES classifier;
    std::string name;
    GType** subTypes;
  } GType;

  GType* getArrayType(GType* elementType);
  GType* getBoolType();
  GType* getCharType();
  GType* getFloatType();
  GType* getInt32Type();
  GType* getStringType();
  GType* getNoneType();
}

#endif

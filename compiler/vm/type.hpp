#include <string>

#ifndef VM2_TYPE_HPP
#define VM2_TYPE_HPP

namespace VM {

  enum BASICTYPES {
    ARRAY,
    BOOL,
    CLASS,
    INT32,
    STRING,
    NONE
  };

  typedef struct {
    BASICTYPES classifier;
    std::string name;
  } GType;

  GType* getArrayType();
  GType* getBoolType();
  GType* getInt32Type();
  GType* getStringType();
  GType* getNoneType();
}

#endif

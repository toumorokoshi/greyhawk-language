#include <string>

#ifndef VM2_TYPE_HPP
#define VM2_TYPE_HPP

namespace VM {

  enum BASICTYPES {
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

  GType* getBoolType();
  GType* getInt32Type();
  GType* getStringType();
  GType* getNoneType();
}

#endif

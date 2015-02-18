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
    FILEHANDLE,
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
  GType* getClassType();
  GType* getFloatType();
  GType* getInt32Type();
  GType* getNoneType();
  GType* getFileHandleType();
  GType* getStringType();
}

#endif

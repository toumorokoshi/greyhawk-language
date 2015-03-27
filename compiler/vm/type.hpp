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
    FUNCTION,
    INSTANCE,
    INT32,
    FILEHANDLE,
    NONE
  };

  struct GType;

  typedef struct GType {
    BASICTYPES classifier;
    std::string name;
    // todo: rename to attributeTypes
    GType** subTypes;
    std::string* attributeNames;
    int subTypeCount;
  } GType;

  GType* getArrayType(GType* elementType);
  GType* getBoolType();
  GType* getCharType();
  GType* getClassType();
  GType* getFloatType();
  GType* getFunctionType();
  GType* getInt32Type();
  GType* getNoneType();
  GType* getFileHandleType();
  GType* getStringType();
}

#endif

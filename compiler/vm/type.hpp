#include <string>

#ifndef VM2_TYPE_HPP
#define VM2_TYPE_HPP

namespace VM {

  struct GFunction;
  class GEnvironment;
  struct GEnvironmentInstance;

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

  /*
    some notes about GType:
    instances of types are just environment instances that expose only their locals.
    they work by have each method instantiate with that instance as the scope.

    assumptions are made regarding the order of the registers of an environmental
    scope. The order is:

    * attributes
    * methods

    this order is required, because an instantiate will automatically populate
    functions bound to the internal scope of the instance.
   */
  typedef struct GType {
    BASICTYPES classifier;
    std::string name;
    // todo: rename to attributeTypes
    // GType** subTypes;
    // std::string* attributeNames;
    int attributeCount;
    // this is necessary to invoke methods
    GEnvironmentInstance* parentEnv;
    GEnvironment* environment;
    // GFunction** functions;
    int functionCount;
    GEnvironmentInstance* instantiate();
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

#include <string>
#include "../std/gstd.hpp"

#ifndef VM2_TYPE_HPP
#define VM2_TYPE_HPP

namespace VM {

  struct GFunction;
  class GEnvironment;
  struct GEnvironmentInstance;

  struct GType;

  /*
    some notes about GType:
    instances of types are just environment instances that expose only their locals.
    they work by have each method instantiate with that instance as the scope.

    assumptions are made regarding the order of the registers of an environmental
    scope. The order is:

    * attributes
    * methods

    this order is required, because an instantiate() will automatically populate
    functions bound to the internal scope of the instance.
   */
  typedef struct GType {
    std::string name;
    // subtypes are kind of a weird thing. right now, they're
    // only used for arrays, to indicate the array element type.
    // at some point, we may want to use these for things like generics.
    gstd::Array<GType*> subTypes;
    int attributeCount;
    int functionCount;
    // this is necessary to invoke methods
    GEnvironmentInstance* parentEnv;
    GEnvironment* environment;
    // this is actually what contains
    // the variables.
    GEnvironmentInstance* instantiate();
  } GType;

  GType* getArrayType(GType* elementType);
  GType* getBoolType();
  GType* getBuiltinType();
  GType* getCharType();
  GType* getClassType();
  GType* getFloatType();
  GType* getFunctionType();
  GType* getModuleType();
  GType* getInt32Type();
  GType* getNoneType();
  GType* getFileHandleType();
  GType* getStringType();
  GType* getTupleType(gstd::Array<GType*>);
}

#endif

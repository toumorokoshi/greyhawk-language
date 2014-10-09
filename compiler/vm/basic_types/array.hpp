#include "../object.hpp"
#include "int.hpp"
#include "string.hpp"
#include <iostream>

#ifndef VM_HPP_BASIC_TYPES_ARRAY
#define VM_HPP_BASIC_TYPES_ARRAY

namespace VM {

  VMClass* getVMArrayClass();

  class VMArray: public VMObject {
  public:
    VMClass* getElementType() { return elementType; }
    virtual VMClass* getType() { return getVMArrayClass(); }
    virtual VMObject* evaluate(VMScope&) { return this; }
    VMObject* next();

    VMObject* __get(VMObjectList& args) {
      auto index = ((VMInt*) args[0])->value;
      return elements[index];
    }

    VMObject* __set(int index, VMObject* value) {
      elements[index] = value; return NULL;
    }
    VMObject* toString() {
      std::string stream = "[";
      static std::vector<VMObject*> noArgs;
      for (int i = 0; i < size; i++) {
        stream += ((VMString*) (elements[i]->call("toString", noArgs)))->value;
        if (i < size - 1) { stream += ", "; }
      }
      stream += "]";
      return new VMString(stream);
    }

    VMObject* call(std::string methodName, std::vector<VMObject*>& args) {

      if (methodName == "__get") { return __get(args); }
      else if (methodName == "toString") { return toString(); }

      throw VMException("array has no method " + methodName);

    }

    VMArray(VMClass* _elementType, int _size, VMObject** _elements) :
      elementType(_elementType), size(_size), elements(_elements) {}

  private:
    VMClass* elementType;
    int size;
    VMObject** elements; // this is an array
  };
}

#endif

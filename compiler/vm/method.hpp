#include "class.hpp"
#include "object.hpp"

#ifndef VM_METHOD_HPP
#define VM_METHOD_HPP

namespace VM {

  VMClass* getVMMethodClass();

  typedef VMObject* (*VMRawMethod)(VMObject*, VMObjectList&);

  class VMMethod : public VMObject {
  public:
    virtual VMClass* getType() { return getVMMethodClass(); }
    VMObject* call(VMObject* self, VMObjectList& arguments);
    VMMethod(std::vector<VMClass*>& argumentTypes, VMRawMethod rawMethod);
  private:
    std::vector<VMClass*>& _argumentTypes;
    VMRawMethod _rawMethod;
  };

}

#endif

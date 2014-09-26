#include "../class.hpp"
#include "../object.hpp"

#ifndef VM_INT_HPP
#define VM_INT_HPP

namespace VM {

  VMClass* getVMIntClass();

  class VMInt : public VMObject {
  public:
    int value;
    virtual VMClass* getType() { return getVMIntClass(); };
    VMInt(int _value): value(_value) {}
  };
}

#endif
